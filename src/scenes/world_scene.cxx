#include "world_scene.hxx"

j_world_scene::j_world_scene() {
    auto player { registry_.create() };

    registry_.assign<jc_position>(player, 10, 10, 0);
    registry_.assign<jc_renderable>(player, static_cast<unsigned char>('@'));
}

void j_world_scene::update() {
    registry_.view<jc_position>().each([](auto& position) {
        position.x++;

        if (position.x >= 50) {
            position.x = 0;
            position.y++;
            if (position.y > 20) {
                position.y = 10;
            }
        }
    });
}

void j_world_scene::render(j_display& display) {
    registry_.view<jc_renderable, jc_position>().each([&display](auto& renderable, auto& position) {
        j_position<uint32_t> pos {
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
