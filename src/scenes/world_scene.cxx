#include "world_scene.hxx"

j_world_scene::j_world_scene() : j_scene(j_scene_type::world) {
}

void j_world_scene::on_create() {
    auto& reg { scene_writer_->registry() };

    player_ = reg.create();
    reg.assign<jc_position>(player_, 0, 1, 0);
    reg.assign<jc_controllable>(player_);
    reg.assign<jc_renderable>(player_, static_cast<unsigned char>('@'));
    reg.assign<jc_attribute_bearing>(player_);
    reg.assign<jc_item_container>(player_, game_events_);


    auto item { reg.create() };
    reg.assign<jc_position>(item, 5, 5, 0);
    reg.assign<jc_renderable>(item, static_cast<unsigned char>('$'), j_color(255, 255, 0));
    item = reg.create();
    reg.assign<jc_position>(item, 16, 4, 0);
    reg.assign<jc_renderable>(item, static_cast<unsigned char>('/'), j_color(255, 255, 0));
}

void j_world_scene::update(j_input_state& input) {
    auto& keyboard { input.keyboard() };
    auto& reg { scene_writer_->registry() };

    j_vec2<int32_t> vel;

    if (keyboard.consume(SDL_KeyCode::SDLK_s)) {
        auto s { static_cast<j_status_scene*> (scene_writer_->load(j_scene_type::status)) };
        s->configure(&reg, &player_);
        return;
    }
    if (keyboard.consume(SDL_KeyCode::SDLK_i)) {
        auto s { static_cast<j_inventory_scene*> (scene_writer_->load(j_scene_type::inventory)) };
        s->configure(&reg, player_);
        return;
    }

    if (keyboard.consume(SDL_KeyCode::SDLK_UP)) {
        vel.y -= 1;
    }
    if (keyboard.consume(SDL_KeyCode::SDLK_DOWN)) {
        vel.y += 1;
    }
    if (keyboard.consume(SDL_KeyCode::SDLK_LEFT)) {
        vel.x -= 1;
    }
    if (keyboard.consume(SDL_KeyCode::SDLK_RIGHT)) {
        vel.x += 1;
    }

    if (keyboard.consume(SDL_KeyCode::SDLK_PLUS)) {
        auto& attr = reg.get<jc_attribute_bearing>(player_);
        attr.modify(j_attribute_type::strength, 2);
    }
    if (keyboard.consume(SDL_KeyCode::SDLK_MINUS)) {
        auto& attr = reg.get<jc_attribute_bearing>(player_);

        attr.modify(j_attribute_type::strength, -2);
    }

    if (!vel.x && !vel.y) {
        return;
    }

    reg.view<jc_controllable, jc_position>().each([&vel = std::as_const(vel)](auto& controllable, auto& position) {
        position.x += vel.x;
        position.y += vel.y;
    });
}

void j_world_scene::render(j_display& display) {
    auto& reg { scene_writer_->registry() };

    reg.view<jc_renderable, jc_position>().each([&display](auto& renderable, auto& position) {
        j_vec2<uint32_t> pos {
            static_cast<uint32_t> (position.x),
            static_cast<uint32_t> (position.y)
        };

        if (position.x < 0 || position.y < 0 || !display.in_bounds(pos)) {
            return;
        }

        j_display_cell cell;

        cell.color = renderable.color;
        cell.glyph = renderable.glyph;

        display.put(std::move(cell), std::move(pos));
    });
}
