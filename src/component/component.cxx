#include "component/component.hxx"
#include "entity/entity.hxx"

Component::Component(ComponentType type) : type_(type) {
}

ComponentType Component::type() const {
    return type_;
}

void Component::set_owner(Entity* entity) {
    if (!entity) {
        return;
    }
    entity_ = entity;
    on_owner_updated();
}

void Component::update() {
}

void Component::on_owner_updated() {
}

void Component::on_after_actions() {
}

void Component::on_player_attached() {
}

void Component::on_player_detached() {
}
