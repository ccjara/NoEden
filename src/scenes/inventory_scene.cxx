#include "inventory_scene.hxx"

j_inventory_scene::j_inventory_scene() : j_scene(j_scene_type::inventory) {
    is_blocking_ = true;
    is_opaque_ = true;
    pagination_.set_page_size(26);
}

void j_inventory_scene::configure(entt::registry* registry, entt::entity entity) {
    assert(registry);
    registry_ = registry;
    entity_ = entity;
}

void j_inventory_scene::update(j_input_state& input) {
    auto& k { input.keyboard() };
    auto& inv { registry_->get<jc_item_container>(entity_) };

    if (k.consume(SDL_KeyCode::SDLK_c)) {
        j_item item;
        item.label = "Skeleton Key";
        inv.put(std::move(item));
    }

    pagination_.update(inv.items().size());

    if (k.consume(SDL_KeyCode::SDLK_ESCAPE)) {
        scene_writer_->unload(id());
        return;
    }

    alpha_select_ = k.is_pressed(SDL_KeyCode::SDLK_LALT);

    if (!alpha_select_ && pagination_.pages()) {
        if (k.consume(SDL_KeyCode::SDLK_UP)) {
            if (!selected_index_) {
                selected_index_ = pagination_.max_index();
            } else {
                --selected_index_;
            }
            pagination_.set_page_by_index(selected_index_);
        } else if (k.consume(SDL_KeyCode::SDLK_DOWN)) {
            if (selected_index_ == pagination_.max_index()) {
                selected_index_ = 0;
            } else {
                ++selected_index_;
            }
            pagination_.set_page_by_index(selected_index_);
        }
    }
}

void j_inventory_scene::render(j_display& display) {
    if (entity_ == entt::null) {
        display.text("There is no inventory information available", j_rect<uint32_t>(2, 0, 0, 2));
        return;
    }
    auto& inv { registry_->get<jc_item_container>(entity_) };
    const auto& items { inv.items() };
    pagination_.update(items.size());

    j_text_options opt;

    opt.boundary.left = 0;
    opt.boundary.top = 0;

    display.text("Inventory", opt);

    constexpr uint32_t x { 0 };
    uint32_t y { 2 };

    if (items.empty()) {
        display.text("Does not hold any items", j_rect<uint32_t>(y, 0, 0, x));
        return;
    }

    if (pagination_.page() > 0) {
        display.put({ 671, j_color::green() }, { x, y });
    }

    ++y;

    pagination_.each(items, [this, &display, x, &y](const auto& item, uint32_t index, uint32_t n) {
        const bool is_selected { selected_index_ == index };
        j_color color { j_color::white() };

        if (alpha_select_) { // render alphabetic  key
            display.put({ 97 + static_cast<int32_t> (n), j_color::green() }, { x, y });
        } else {
            if (is_selected) { // render selection indicator
                display.put({ 618, j_color::green() }, { x, y });
            } else {
                color = j_color::mono(200);
            }
        }
        display.text(item.label, j_rect<uint32_t>(y, 0, 0, x + 2), color);

        ++y;
    });

    if (!pagination_.is_last_page()) {
        display.put({ 673, j_color::green() }, { x, y });

        char more[32];
        std::snprintf(more, 32, "(%i more)", pagination_.remaining_entries());
        display.text(more, j_rect<uint32_t>(y, 0, 0, x + 2), j_color::white());
    }
}
