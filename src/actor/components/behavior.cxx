#include "behavior.hxx"

Behavior::Behavior() : Component(ComponentType::Behavior) {
}

std::unique_ptr<Component> Behavior::clone() const {
    return std::make_unique<Behavior>(*this);
}
