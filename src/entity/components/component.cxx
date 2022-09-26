#include "component.hxx"

Component::Component(ComponentType type) : type_(type) {
}

ComponentType Component::type() const {
    return type_;
}

u64 Component::entity_id() const {
    return entity_id_;
}

void Component::update(u64 dt) {
}
