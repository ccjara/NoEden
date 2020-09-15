#include "systems.hxx"

j_systems::j_systems(entt::dispatcher* dispatcher) :
    dispatcher_(dispatcher) {
    assert(dispatcher_);
}

void j_systems::update(float_t delta_time) {
    for (auto& i : systems_) {
        i.second->update(delta_time);
    }
}
