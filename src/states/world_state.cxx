#include "world_state.hxx"

void j_world_state::on_enter() {
    auto entities { game->entities() };

    dwarf_ = entities->create();
    entities->assign<jc_position>(dwarf_, 0, 1, 0);
    entities->assign<jc_renderable>(dwarf_, jc_renderable::unit('@', j_color::white()));
    entities->assign<jc_attribute_bearing>(dwarf_);
    entities->assign<jc_item_container>(dwarf_, game->events());

    game->events()->trigger<j_player_control_event>(dwarf_);

    troll_ = entities->create();
    entities->assign<jc_position>(troll_, 5, 5, 0);
    entities->assign<jc_renderable>(troll_, jc_renderable::unit('T', j_color::red()));
    entities->assign<jc_attribute_bearing>(troll_);
    entities->assign<jc_item_container>(troll_, game->events());

    auto item { entities->create() };
    entities->assign<jc_position>(item, 2, 3, 0);
    entities->assign<jc_renderable>(item, jc_renderable::item('$', j_color::yellow()));
    entities->assign<jc_object_descriptor>(item, "Gold coin");
    entities->assign<jc_item>(item, "Gold coin");
}

void j_world_state::on_update() {

}

void j_world_state::on_leave() {

}