#include "actor.hxx"

Actor::Actor() : id(next_id_++) {
}

void Actor::reindex_components() {
    components_by_type_.clear();
    for (auto& component : components_) {
        components_by_type_[component->type()] = component.get();
    }
}
