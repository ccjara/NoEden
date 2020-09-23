#include "journal.hxx"

void j_journal::attach(entt::dispatcher* dispatcher) {
    assert(dispatcher);
    dispatcher->sink<j_gathering_completed_event>().connect<&j_journal::on_gathering_completed>(this);
}

void j_journal::on_gathering_completed(const j_gathering_completed_event& e) {
    std::string log_str { "Picked up " };

    if (game->entities()->has<jc_object_descriptor>(e.item)) {
        log_str += game->entities()->get<jc_object_descriptor>(e.item).label;
    } else {
        log_str += " something";
    }

    entries_.emplace_back(std::move(log_str));
}

const std::vector<std::string>& j_journal::entries() const {
    return entries_;
}
