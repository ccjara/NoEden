#include "world_state.hxx"

void j_world_state::on_enter() {
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
    entities->assign<jc_item>(item, "Gold coin");

    item = entities->create();
    entities->assign<jc_position>(item, 8, 4);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color(0x888888) });
    entities->assign<jc_object_descriptor>(item, "Silver coin");
    entities->assign<jc_item>(item, "Silver coin");

    item = entities->create();
    entities->assign<jc_position>(item, 6, 1);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color(0x875630) });
    entities->assign<jc_object_descriptor>(item, "Copper coin");
    entities->assign<jc_item>(item, "Copper coin");
}

void j_world_state::on_update() {

}

void j_world_state::on_leave() {

}
