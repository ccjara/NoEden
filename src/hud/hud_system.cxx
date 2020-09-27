#include "hud_system.hxx"

void j_hud_system::on_load() {
    define_task<j_gathering_completed_event, &j_hud_system::task_journal_item_pickup>();
}

void j_hud_system::update(uint32_t delta_time) {
    queue_.update();
}

const std::vector<std::string>& j_hud_system::journal_entries() const {
    return journal_.entries();
}

void j_hud_system::task_journal_item_pickup(const j_gathering_completed_event& e) {
    std::string log_str { "Picked up " };

    if (game->entities()->has<jc_object_descriptor>(e.item)) {
        log_str += "$c00ff00";
        log_str += game->entities()->get<jc_object_descriptor>(e.item).label;
        log_str += "$!";
    } else {
        log_str += " something";
    }

    journal_.emplace_back(std::move(log_str));
}
