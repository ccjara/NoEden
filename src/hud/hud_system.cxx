#include "hud_system.hxx"

void j_hud_system::on_load() {
    status_.reserve(128);

    journal_.attach(game->events());

    game->events()->sink<j_player_moved_event>().connect<&j_hud_system::on_player_movement>(this);

    auto entities { game->entities() };
    item_observer_.connect(*entities, entt::collector.group<jc_item, jc_renderable, jc_position, jc_object_descriptor>());
}

void j_hud_system::on_player_movement(const j_player_moved_event& e) {
    auto entities { game->entities() };

    // see if the player stepped on an item (will use map later)
    for (const auto item : item_observer_) {
        auto& pos { entities->get<jc_position>(item) };
        auto& desc { entities->get<jc_object_descriptor>(item) };

        if (pos.x == e.position.x && pos.y == e.position.y) {
            status_ = desc.label + " lies on the ground";
            return;
        }
    }
    status_ = "";
}

void j_hud_system::update(uint32_t delta_time) {

}

const std::vector<std::string>& j_hud_system::journal_entries() const {
    return journal_.entries();
}

const std::string& j_hud_system::status() const {
    return status_;
}
