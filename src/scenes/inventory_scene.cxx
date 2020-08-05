#include "inventory_scene.hxx"

j_inventory_scene::j_inventory_scene() : j_base_scene(j_scene_type::inventory) {
    is_blocking_ = true;
    is_opaque_ = true;
}

void j_inventory_scene::configure(entt::registry* registry, entt::entity* entity) {
    assert(entity);
    assert(registry);
    entity_ = entity;
    registry_ = registry;
}

void j_inventory_scene::update(j_input_state& input) {
    if (input.keyboard().consume(SDL_KeyCode::SDLK_ESCAPE)) {
        scene_writer_->unload(id());
        return;
    }

    if (input.keyboard().consume(SDL_KeyCode::SDLK_c)) {
        auto& inv { registry_->get<jc_item_container>(*entity_) };

        j_item item;

        item.label = "Dimensional Slicer";
        item.quality = j_item_quality::common;

        game_events_->trigger<j_inventory_item_added_event>(&item);

        inv.put(std::move(item));
    }

    if (!entity_) {
        return;
    }
}

void j_inventory_scene::render(j_display& display) {
    if (!entity_) {
        display.text("There is no inventory information available", j_rect<uint32_t>(2, 0, 0, 2));
        return;
    }
    j_text_options opt;

    opt.boundary.left = 0;
    opt.boundary.top = 0;

    display.text("Inventory", opt);

    auto& inv { registry_->get<jc_item_container>(*entity_) };

    uint32_t top = 2;

    constexpr auto item_name = [](const j_item& item) -> std::string {
        std::string ret;

        switch (item.quality) {
        case j_item_quality::common:
        default:
            break;
        case j_item_quality::flawed:
            ret += "Flawed ";
            break;
        case j_item_quality::sublime:
            ret += "Sublime ";
            break;
        case j_item_quality::marvellous:
            ret += "Marvellous ";
            break;
        case j_item_quality::exceptional:
            ret += "Exceptional ";
            break;
        case j_item_quality::supreme:
            ret += "Supreme ";
            break;
        }

        ret += item.label;

        return ret;
    };

    for (const auto& [id, item] : inv.items()) {
        const auto name { item_name(item) };

        display.text(
            name,
            j_rect<uint32_t>(top, 0, 0, 0)
        );

        top++;
    }
}
