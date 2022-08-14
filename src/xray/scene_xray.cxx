#include "scene_xray.hxx"

SceneXray::SceneXray(
    EventManager& dispatcher,
    PlayerController& player_controller,
    Renderer& renderer
) : events_ { dispatcher },
    player_controller_ { player_controller },
    renderer_ { renderer } {
    events_.on<SceneLoadedEvent>(this, &SceneXray::on_scene_loaded);
    events_.on<MouseDownEvent>(this, &SceneXray::on_mouse_down, 9000);
}

bool SceneXray::on_scene_loaded(SceneLoadedEvent& e) {
    scene_ = e.scene;
    return false;
}

bool SceneXray::on_mouse_down(MouseDownEvent& e) {
    if (!scene_ || !tile_window_data_.is_editing) {
        return false;
    }

    TileType type_to_place;
    if (e.state->is_mouse_pressed(MouseButton::Left)) {
        type_to_place = tile_window_data_.lmb_type;
    } else if (e.state->is_mouse_pressed(MouseButton::Right)) {
        type_to_place = tile_window_data_.rmb_type;
    } else {
        return false;
    }
    const auto mp = e.state->mouse_position();
    const Vec2<u32> tpos = { mp.x / 16, mp.y / 28 }; // TODO
    if (!scene_->tiles().at(tpos)) {
        return false;
    }
    auto w = TileBuilder::for_type(type_to_place);
    w.revealed = true;
    scene_->tiles().put(w, tpos);
    scene_->update_fov(player_controller_.player());
    return true;
}

void SceneXray::update() {
    if (!scene_) {
        return;
    }
    entity_window();
    tile_window();
}

void SceneXray::entity_window() {
    static std::optional<u64> actor_id = std::nullopt;
    static std::string combo_label = "Select actor";

    ImGui::Begin("Entities");

    if (ImGui::BeginCombo("Entity", combo_label.c_str())) {
        for (auto& actor : scene_->actors()) {
            const auto& actor_name_str = actor->archetype->name.c_str();
            const bool is_selected = actor_id.has_value() && actor_id.value() == actor->id;
            if (ImGui::Selectable(actor_name_str, is_selected)) {
                actor_id = actor->id;
                combo_label = actor_name_str;
            }
        }
        ImGui::EndCombo();
    }
    actor_panel(actor_id);

    ImGui::End();
}

void SceneXray::tile_window() {
    const static std::unordered_map<TileType, std::string> type_options = {
        { TileType::None, "None" },
        { TileType::Floor, "Floor" },
        { TileType::Wall, "Wall" },
        { TileType::Water, "Water" },
    };

    ImGui::Begin("Tiles");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4);
    if (ImGui::BeginCombo("LMB Tile", type_options.at(tile_window_data_.lmb_type).c_str())) {
        for (auto& option : type_options) {
            const bool is_selected = tile_window_data_.lmb_type == option.first;
            if (ImGui::Selectable(option.second.c_str(), is_selected)) {
                tile_window_data_.lmb_type = option.first;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::BeginCombo("RMB Tile", type_options.at(tile_window_data_.rmb_type).c_str())) {
        for (auto& option : type_options) {
            const bool is_selected = tile_window_data_.rmb_type == option.first;
            if (ImGui::Selectable(option.second.c_str(), is_selected)) {
                tile_window_data_.rmb_type = option.first;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth(); // select width
    ImGui::Checkbox("Edit Mode", &tile_window_data_.is_editing);
    ImGui::End();
}

void SceneXray::actor_panel(std::optional<u64> actor_id) {
    if (!actor_id.has_value()) {
        return;
    }
    Actor* actor = scene_->get_actor_by_id(actor_id.value());
    if (actor == nullptr) {
        return;
    }
    actor_glyph(actor);

    i32 position_raw[2] = { actor->position.x, actor->position.y };
    bool is_player = player_controller_.player() == actor;

    ImGui::Text("Id: %llx", actor->id);
    if (ImGui::Checkbox("Player", &is_player)) {
        player_controller_.control(is_player ? actor : nullptr);
    }
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4);
    if (ImGui::InputInt2("Position", position_raw, ImGuiInputTextFlags_None)) {
        position_raw[0] = std::min(std::max(position_raw[0], 0), 100);
        position_raw[1] = std::min(std::max(position_raw[1], 0), 100);
        actor->position.x = position_raw[0];
        actor->position.y = position_raw[1];
        scene_->update_fov(player_controller_.player());
    }
    if (ImGui::InputInt("Speed", &actor->speed, ImGuiInputTextFlags_None)) {
        actor->speed = std::max(actor->speed, 0);
    }
    if (ImGui::InputInt("Energy", &actor->energy, ImGuiInputTextFlags_None)) {
        actor->energy = std::max(actor->energy, 0);
    }
    for (auto& [id, skill] : actor->skills) {
        const auto label = Translator::translate(skill.label());
        if (ImGui::InputInt(label.c_str(), &skill.progress)) {
            skill.progress = std::max(skill.progress, 0);
        }
    }
    ImGui::PopItemWidth();
}

void SceneXray::actor_glyph(Actor* actor) {
    if (actor == nullptr) {
        return;
    }
    const auto& display_info = actor->archetype->display_info;
    const auto uvuv = renderer_.calculate_glyph_uv(display_info.glyph);
    const auto texture = renderer_.text_texture();
    const auto& color = display_info.color;
    const float glyph_width = 32.0f;
    ImGui::Image(
        reinterpret_cast<void*>(static_cast<intptr_t>(texture)),
        ImVec2(glyph_width, glyph_width / renderer_.glyph_aspect_ratio()),
        ImVec2(uvuv[0], uvuv[1]),
        ImVec2(uvuv[2], uvuv[3]),
        ImVec4(
            static_cast<float>(color.r) / 255.0f,
            static_cast<float>(color.b) / 255.0f,
            static_cast<float>(color.g) / 255.0f,
            1.0f
        )
    );
}
