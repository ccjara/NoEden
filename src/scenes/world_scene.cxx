#include "world_scene.hxx"

j_world_scene::j_world_scene() {
    player_ = registry_.create();

    registry_.assign<jc_position>(player_, 10, 10, 0);
    registry_.assign<jc_renderable>(player_, static_cast<unsigned char>('@'));
    registry_.assign<jc_attribute_bearing>(player_);
}

void j_world_scene::update(j_input_state& input) {
    j_vec2<int32_t> vel;

    if (input.keyboard().consume(SDL_KeyCode::SDLK_s)) {
        auto s { static_cast<j_status_scene*> (scene_writer_->load(j_scene_type::status)) };

        s->configure(&registry_, &player_);
    }

    if (input.keyboard().consume(SDL_KeyCode::SDLK_UP)) {
        vel.y -= 1;
    }
    if (input.keyboard().consume(SDL_KeyCode::SDLK_DOWN)) {
        vel.y += 1;
    }
    if (input.keyboard().consume(SDL_KeyCode::SDLK_LEFT)) {
        vel.x -= 1;
    }
    if (input.keyboard().consume(SDL_KeyCode::SDLK_RIGHT)) {
        vel.x += 1;
    }

    if (input.keyboard().consume(SDL_KeyCode::SDLK_PLUS)) {
        auto& attr = registry_.get<jc_attribute_bearing>(player_);

        attr.modify(j_attribute_type::strength, 2);
    }
    if (input.keyboard().consume(SDL_KeyCode::SDLK_MINUS)) {
        auto& attr = registry_.get<jc_attribute_bearing>(player_);

        attr.modify(j_attribute_type::strength, -2);
    }

    if (!vel.x && !vel.y) {
        return;
    }

    registry_.view<jc_position>().each([&vel = std::as_const(vel)](auto& position) {
        position.x += vel.x;
        position.y += vel.y;
    });
}

void j_world_scene::render(j_display& display) {
    registry_.view<jc_renderable, jc_position>().each([&display](auto& renderable, auto& position) {
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
