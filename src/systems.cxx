#include "systems.hxx"

j_systems::j_systems(entt::dispatcher* dispatcher) :
    dispatcher_(dispatcher) {
    assert(dispatcher_);
}

j_systems::~j_systems() {
    unload();
}

void j_systems::update(float_t delta_time) {
    for (auto& i : systems_) {
        i->update(delta_time);
    }
}

void j_systems::unload() {
    for (auto iter = systems_.rbegin(); iter != systems_.rend(); ++iter) {
        iter->get()->on_unload();
    }
    systems_by_id_.clear();
    systems_.clear();
}