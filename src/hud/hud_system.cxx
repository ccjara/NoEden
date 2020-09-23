#include "hud_system.hxx"

void j_hud_system::on_load() {
    journal_.attach(game->events());

    game->events()->sink<j_player_moved_event>().connect<&j_hud_system::on_player_movement>(this);

    auto entities { game->entities() };

    status_text_ = game->entities()->create();

    item_observer_.connect(*entities, entt::collector.group<jc_item, jc_renderable, jc_position, jc_object_descriptor>());

    entities->assign<jc_position>(status_text_, 1, 0, 0);
    auto& renderable { entities->assign<jc_renderable>(status_text_, "", j_text_options{}) };
    renderable.text.reserve(128);
}

void j_hud_system::on_player_movement(const j_player_moved_event& e) {
    auto entities { game->entities() };
    auto& renderable { entities->get<jc_renderable>(status_text_) };

    // see if the player stepped on an item (will use map later)
    for (const auto item : item_observer_) {
        auto& pos { entities->get<jc_position>(item) };
        auto& desc { entities->get<jc_object_descriptor>(item) };

        if (pos.x == e.position.x && pos.y == e.position.y) {
            renderable.text = desc.label + " lies on the ground";
            return;
        }
    }
    renderable.text = "Nothing is here";
}

void j_hud_system::update(uint32_t delta_time) {

}

const std::vector<std::string>& j_hud_system::journal_entries() const {
    return journal_.entries();
}
