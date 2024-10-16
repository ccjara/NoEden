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

static const std::unordered_map<std::string_view, ComponentType> component_type_map {
    {"skills", ComponentType::Skills},
    {"behavior", ComponentType::Behavior},
    {"render", ComponentType::Render},
    {"vision", ComponentType::Vision},
    {"life", ComponentType::Life},
};

ComponentType parse_component_type(std::string_view value) {
    if (const auto it = component_type_map.find(value); it != component_type_map.cend()) {
        return it->second;
    }
    return ComponentType::Invalid;
}
