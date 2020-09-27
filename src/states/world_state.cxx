#include "world_state.hxx"

void j_world_state::on_enter() {
    layer_.resize({ 100, 100 });

    for (auto& cell : layer_.cells()) {
        cell.displayed_cell = j_display_cell {
            '.',
            j_color::mono(90)
        };
    }

    auto entities { game->entities() };

    dwarf_ = entities->create();
    entities->assign<jc_position>(dwarf_, 0, 1);
    entities->assign<jc_renderable>(dwarf_, jc_renderable { '@', j_color::white() });
    entities->assign<jc_attribute_bearing>(dwarf_);
    auto& inventory { entities->assign<jc_inventory>(dwarf_) };

    inventory.create_container();

    game->events()->trigger<j_player_control_event>(dwarf_);

    troll_ = entities->create();
    entities->assign<jc_position>(troll_, 5, 5);
    entities->assign<jc_renderable>(troll_, jc_renderable { 'T', j_color::red() });
    entities->assign<jc_attribute_bearing>(troll_);

    auto item { entities->create() };
    entities->assign<jc_position>(item, 2, 3);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color::yellow() });
    entities->assign<jc_object_descriptor>(item, "Gold coin");
    entities->assign<jc_item>(item);

    item = entities->create();
    entities->assign<jc_position>(item, 8, 4);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color(0x888888) });
    entities->assign<jc_object_descriptor>(item, "Silver coin");
    entities->assign<jc_item>(item);

    item = entities->create();
    entities->assign<jc_position>(item, 6, 1);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color(0x875630) });
    entities->assign<jc_object_descriptor>(item, "Copper coin");
    entities->assign<jc_item>(item);
}

void j_world_state::on_update() {
}

void j_world_state::on_render(j_display& display) {
    j_vec2<uint32_t> pos;
    // TODO: assuming camera offset 0 for now
    for (; pos.y < display.dimensions().y; ++pos.y) {
        for (; pos.x < display.dimensions().x; ++pos.x) {
            const auto& world_cell = layer_.at(pos, fast_grid_access_tag {});

            display.put(world_cell.displayed_cell, pos, fast_grid_access_tag {});
        }
        pos.x = 0;
    }

    game->entities()->view<jc_renderable, jc_position>().each([&display](auto& renderable, auto& position) {
        if (!display.in_bounds(position)) {
            return; // do not render entities outside of view
        }
        display.put(
            j_display_cell(renderable.glyph, renderable.color),
            position,
            fast_grid_access_tag {} // bounds checked above
        );
    });

    render_hud(display);
}

void j_world_state::on_leave() {
}

const j_grid<j_tile>& j_world_state::layer() const {
    return layer_;
}

void j_world_state::render_hud(j_display& display) {
    auto hud { game->systems()->get<j_hud_system>() };
    const auto& journal { hud->journal_entries() };

    j_vec2<uint32_t> pos { 0, display.dimensions().y - 1 };

    for (auto it { journal.crbegin() }; it != journal.crend(); ++it) {
        display.text(*it, pos);

        if (--pos.y == 0) {
            break;
        }
    }
}
