#include "world_state.hxx"
#include "../game.hxx"

void j_world_state::on_enter() {
    auto entities { game->entities() };

    auto player_ = entities->create();
    entities->assign<jc_position>(player_, 0, 1, 0);
    entities->assign<jc_controllable>(player_);
    entities->assign<jc_renderable>(player_, static_cast<unsigned char>('@'));
    entities->assign<jc_attribute_bearing>(player_);
    entities->assign<jc_item_container>(player_, game->events());

    game->events()->trigger<j_player_control_event>(player_);

    auto item { entities->create() };
    entities->assign<jc_position>(item, 5, 5, 0);
    entities->assign<jc_renderable>(item, static_cast<unsigned char>('$'), j_color(255, 255, 0));
    item = entities->create();
    entities->assign<jc_position>(item, 16, 4, 0);
    entities->assign<jc_renderable>(item, static_cast<unsigned char>('/'), j_color(255, 255, 0));
}

void j_world_state::on_update() {

}

void j_world_state::on_leave() {

}