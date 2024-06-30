#include "scene_xray.hxx"

#include <realm/realm_event.hxx>

#include "component/render.hxx"
#include "component/skills.hxx"
#include "component/vision/vision.hxx"
#include "entity/entity.hxx"
#include "entity/entity_manager.hxx"
#include "framework/noise_generator.hxx"
#include "gfx/renderer.hxx"
#include "realm/realm_type_def.hxx"
#include "tile/tile_manager.hxx"
#include "world/chunk.hxx"
#include "world/chunk_manager.hxx"
#include "world/tile_accessor.hxx"
#include "world/world_realm.hxx"
#include "world/world_spec.hxx"

bool SceneXray::initialize() {
    mouse_down_sub_ = events_->on<MouseDownEvent>(this, &SceneXray::on_mouse_down, 9000);
    config_updated_sub_ = events_->on<ConfigUpdatedEvent>(this, &SceneXray::on_config_updated, 9000);
    realm_loaded_sub_ = events_->on<RealmLoadedEvent>(this, &SceneXray::on_realm_loaded, 9000);
    return true;
}

EventResult SceneXray::on_realm_loaded(const RealmLoadedEvent& e) {
    if (e.realm->type() == RealmType::World) {
        world_context_ = &static_cast<WorldRealm*>(e.realm)->world_context();
    } else {
        world_context_ = nullptr;
    }
    return EventResult::Continue;
}

EventResult SceneXray::on_config_updated(const ConfigUpdatedEvent& e) {
    config_ = e.next;
    return EventResult::Continue;
}

EventResult SceneXray::on_mouse_down(const MouseDownEvent& e) {
    /*
    TileType type_to_place;
    if (input_->is_mouse_pressed(MouseButton::Left)) {
        type_to_place = tile_window_data_.lmb_type;
    } else if (input_->is_mouse_pressed(MouseButton::Right)) {
        type_to_place = tile_window_data_.rmb_type;
    } else {
        return false;
    }
    const auto mp = input_->mouse_position();
    const Vec2<u32> tpos = {
        mp.x / (config_.glyph_size.x * config_.scaling),
        mp.y / (config_.glyph_size.y * config_.scaling)
    };
    if (!tile_manager_->tiles().at(tpos)) {
        return false;
    }
    auto tile = tile_manager_->create_tile(type_to_place, tpos);
    tile.revealed = true;
    tile_manager_->tiles().put(std::move(tile), tpos);
    return true;
    */
    return EventResult::Continue;
}

void SceneXray::render() {
    if (!world_context_) {
        return;
    }
    entity_window(*world_context_);
    tile_window(*world_context_);
    mapgen_window(*world_context_);
}

void SceneXray::entity_window(WorldContext& world_context) {
    static std::optional<u64> entity_id = std::nullopt;
    static std::string combo_label = "Select entity";

    ImGui::Begin("Entities");

    if (ImGui::BeginCombo("Entity", combo_label.c_str())) {
        for (auto& entity: world_context.entity_manager->entities()) {
            const auto& entity_name_str = entity->name.c_str();
            const bool is_selected = entity_id.has_value() && entity_id.value() == entity->id;
            if (ImGui::Selectable(entity_name_str, is_selected)) {
                entity_id = entity->id;
                combo_label = entity_name_str;
            }
        }
        ImGui::EndCombo();
    }
    entity_panel(entity_id, world_context);

    ImGui::End();
}

void SceneXray::tile_window(WorldContext& world_context) {
    /*
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
    */
}

void SceneXray::entity_panel(std::optional<u64> entity_id, WorldContext& world_context) {
    if (!entity_id.has_value()) {
        return;
    }
    auto* translator = svc_->get<Translator>();
    auto* entity_manager = world_context.entity_manager;

    Entity* entity = entity_manager->entity(entity_id.value());
    if (entity == nullptr) {
        return;
    }
    entity_glyph(entity);

    i32 position_raw[3] = { entity->position.x, entity->position.y, entity->position.z };
    bool is_player = entity_manager->player() == entity;

    ImGui::Text("Id: %lx", entity->id);
    if (ImGui::Checkbox("Player", &is_player)) {
        entity_manager->set_controlled_entity(is_player ? entity->id : null_id);
    }
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4);
    if (ImGui::InputInt3("Position", position_raw, ImGuiInputTextFlags_None)) {
        entity->position.x = position_raw[0];
        entity->position.y = position_raw[1];
        entity->position.z = position_raw[2];
    }
    if (ImGui::InputFloat("Speed", &entity->speed, ImGuiInputTextFlags_None)) {
        entity->speed = std::fmax(entity->speed, 0);
    }
    if (ImGui::InputFloat("Energy", &entity->energy, ImGuiInputTextFlags_None)) {
        entity->energy = std::fmax(entity->energy, 0);
    }
    if (ImGui::InputFloat("Energy Reserved", &entity->energy_reserved, ImGuiInputTextFlags_None)) {
        entity->energy_reserved = std::fmax(entity->energy_reserved, 0);
    }

    // TODO: method on component? like component->draw_xray();
    Vision* vision_component = entity->component<Vision>();
    if (vision_component != nullptr) {
        auto radius = vision_component->vision_radius();
        if (ImGui::InputInt("Vision Radius", &radius, ImGuiInputTextFlags_None)) {
            vision_component->set_vision_radius(radius);
        }
        bool apply = vision_component->applied();
        if (ImGui::Checkbox("Vision Applied", &apply)) {
            vision_component->set_apply(apply);
        }
    }

    Skills* skills_component = entity->component<Skills>();
    if (skills_component != nullptr) {
        for (auto& [id, skill]: skills_component->skills()) {
            const auto label = translator->translate(skill.label());
            i32 progress = skill.progress;
            if (ImGui::InputInt(label.c_str(), &progress)) {
                skills_component->set_progress(id, std::max(skill.progress, 0));
            }
        }
    }

    ImGui::PopItemWidth();
}

void SceneXray::entity_glyph(Entity* entity) {
    if (entity == nullptr) {
        return;
    }
    auto renderer = svc_->get<Renderer>();
    auto rc = entity->component<Render>();
    if (!rc) {
        return;
    }
    const auto& display_info = rc->display_info();
    const auto uvuv = renderer->calculate_glyph_uv(display_info.glyph);
    const auto texture = renderer->text_texture();
    const auto& color = display_info.color;
    const float glyph_width = 32.0f;
    ImGui::Image(
        reinterpret_cast<void*>(static_cast<intptr_t>(texture)),
        ImVec2(glyph_width, glyph_width / renderer->glyph_aspect_ratio()),
        ImVec2(uvuv[0], uvuv[1]),
        ImVec2(uvuv[2], uvuv[3]),
        ImVec4(
            static_cast<float>(color.r) / 255.0f,
            static_cast<float>(color.g) / 255.0f,
            static_cast<float>(color.b) / 255.0f,
            1.0f
        )
    );
}

void SceneXray::mapgen_window(WorldContext& world_context) {
    auto* entity_manager = world_context.entity_manager;
    auto* tile_accessor = world_context.tile_accessor;
    auto* chunk_manager = world_context.chunk_manager;

    ImGui::Begin("Mapgen");

    static GenerateNoiseOptions world_mask_options;

    auto generate_map = [&]() {
        const auto noise = generate_noise(world_mask_options);
        noise_texture_.set_noise(noise, world_mask_options.width, world_mask_options.height);
    };

    static bool init = false;

    if (!init) {
        generate_map();
        init = true;
    }

    if (ImGui::SliderFloat("Frequency", &world_mask_options.frequency, 0.1f, 10.0f)) {
        generate_map();
    }
    if (ImGui::SliderFloat("Amplitude", &world_mask_options.amplitude, 0.1f, 2.0f)) {
        generate_map();
    }
    if (ImGui::SliderFloat("Lacunarity", &world_mask_options.lacunarity, 0.0f, 10.0f)) {
        generate_map();
    }
    if (ImGui::SliderFloat("Gain", &world_mask_options.gain, 0.0f, 10.0f)) {
        generate_map();
    }
    if (ImGui::SliderInt("Offset-X", &world_mask_options.offset_x, 0.0f, 64.0f)) {
        generate_map();
    }
    if (ImGui::SliderInt("Offset-Y", &world_mask_options.offset_y, 0.0f, 64.0f)) {
        generate_map();
    }
    if (ImGui::SliderInt("Octaves", &world_mask_options.octaves, 0.0f, 10)) {
        generate_map();
    }
    if (ImGui::Checkbox("Radial Gradient", &world_mask_options.use_gradient)) {
        generate_map();
    }
    if (world_mask_options.use_gradient && ImGui::SliderFloat("Radius", &world_mask_options.radius_mult, 0.0f, 1.0f)) {
        generate_map();
    }
    if (world_mask_options.use_gradient &&
        ImGui::SliderFloat("Gradient Falloff", &world_mask_options.gradient_falloff, 0.0f, 1.0f)) {
        generate_map();
    }

    noise_texture_.draw();

    if (entity_manager->player() != nullptr) {
        auto p = entity_manager->player()->position;
        auto local_index = tile_accessor->to_local_index(p);
        auto text = fmt::format("Player pos: ({}, {}, {}) - index {} - height {}",
                                p.x, p.y, p.z,
                                local_index,
                                chunk_manager->get_chunk(p)->height_map[p.x % Chunk::CHUNK_SIDE_LENGTH + p.z % Chunk::CHUNK_SIDE_LENGTH * Chunk::CHUNK_SIDE_LENGTH]);

        ImGui::TextUnformatted(text.c_str());
    }

    ImGui::End();
}
