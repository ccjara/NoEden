#include "scene_xray.hxx"

SceneXray::SceneXray(
    EventManager& dispatcher,
    PlayerController& player_controller,
    Renderer& renderer
) : events_ { dispatcher },
    player_controller_ { player_controller },
    renderer_ { renderer } {
    events_.on<SceneLoadedEvent>(this, &SceneXray::on_scene_loaded);
}

bool SceneXray::on_scene_loaded(SceneLoadedEvent& e) {
    scene_ = e.scene;
    return false;
}

void SceneXray::update() {
    static std::optional<u64> actor_id = std::nullopt;
    static std::string combo_label = "Select actor";

    if (!scene_) {
        return;
    }
    ImGui::Begin("Scene");

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
    }
    if (ImGui::InputInt("Speed", &actor->speed, ImGuiInputTextFlags_None)) {
        actor->speed = std::max(actor->speed, 0);
    }
    if (ImGui::InputInt("Energy", &actor->energy, ImGuiInputTextFlags_None)) {
        actor->energy = std::max(actor->energy, 0);
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