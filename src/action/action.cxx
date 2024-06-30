#include "action/action.hxx"

Entity* Action::entity() const {
    return entity_;
}

float Action::cost() const {
    return cost_;
}

float Action::speed() const {
    return speed_;
}

ActionType Action::type() const {
    return type_;
}

void Action::initialize() {
}

 Action::Action(ActionType type) : type_(type) {}
