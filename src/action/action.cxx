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
