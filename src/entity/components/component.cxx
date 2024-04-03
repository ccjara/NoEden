#include "component.hxx"
#include "../entity.hxx"

Component::Component(ComponentType type) : type_(type) {
}

ComponentType Component::type() const {
    return type_;
}

u64 Component::entity_id() const {
    return entity_id_;
}

void Component::set_owner(Entity* entity) {
    if (!entity) {
        entity_id_ = null_id;
        return;
    }
    entity_id_ = entity->id;
}

void Component::update(u64 dt) {
}
