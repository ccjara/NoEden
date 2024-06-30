#ifndef NOEDEN_NULL_ACTION_HXX
#define NOEDEN_NULL_ACTION_HXX

#include "action/action.hxx"

/**
 * @brief Dummy action that does nothing.
 */
class NullAction : public Action {
public:
    NullAction() : Action(ActionType::None) {}

    ActionResult perform() override { return ActionResult::Success; }
};

#endif
