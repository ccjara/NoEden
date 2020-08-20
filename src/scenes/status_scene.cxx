#include "status_scene.hxx"

std::pair<std::string_view, j_color> j_status_scene::attribute_info(const j_attribute& attr) {
    if (!attr.points) {
        return std::make_pair<std::string_view, j_color>("Nonexistent", j_color(128, 0, 0));
    } else if (attr.points < j_attribute_threshold::poor) {
        return std::make_pair<std::string_view, j_color>("Poor", j_color(255, 0, 0));
    } else if (attr.points < j_attribute_threshold::very_low) {
        return std::make_pair<std::string_view, j_color>("Very Low", j_color(255, 128, 0));
    } else if (attr.points < j_attribute_threshold::low) {
        return std::make_pair<std::string_view, j_color>("Low", j_color(255, 255, 0));
    } else if (attr.points < j_attribute_threshold::average) {
        return std::make_pair<std::string_view, j_color>("Average", j_color(255, 255, 255));
    } else if (attr.points < j_attribute_threshold::high) {
        return std::make_pair<std::string_view, j_color>("High", j_color(0, 255, 0));
    } else if (attr.points < j_attribute_threshold::very_high) {
        return std::make_pair<std::string_view, j_color>("Very High", j_color(0, 255, 255));
    } 
    return std::make_pair<std::string_view, j_color>("Inconceivable", j_color(255, 0, 255));
}

j_status_scene::j_status_scene() : j_scene(j_scene_type::status) {
    is_blocking_ = true;
    is_opaque_ = true;
}

void j_status_scene::configure(entt::registry* registry, entt::entity* entity) {
    assert(entity);
    assert(registry);
    entity_ = entity;
    registry_ = registry;
}

void j_status_scene::update(j_input_state& input) {
    if (input.keyboard().consume(SDL_KeyCode::SDLK_ESCAPE)) {
        scene_writer_->unload(id());
    }

    if (!entity_) {
        return;
    }
}

void j_status_scene::render(j_display& display) {
    if (!entity_) {
        display.text("There is no status information available", j_rect<uint32_t>(2, 0, 0, 2));
        return;
    }
    j_text_options opt;

    opt.boundary.left = 0;
    opt.boundary.top = 0;

    display.text("Status", opt);

    auto& target { registry_->get<jc_attribute_bearing>(*entity_) };

    uint32_t top = 2;

    display.text("Attributes", j_rect<uint32_t>(top, 0, 0, 0), j_color(128, 128, 128));

    top += 2;

    for (const auto& [type, attr] : target.attributes()) {
        std::array<char, 64> buf;

        const auto& [tier, color] { attribute_info(attr) };

        std::snprintf(
            buf.data(),
            buf.size(),
            "%s %s",
            tier.data(),
            attr.name.data()
        );

        display.text(
            std::string(buf.data()),
            j_rect<uint32_t>(top, 0, 0, 0),
            color
        );

        top++;
    }
}
