#ifndef NOEDEN_MOVE_ACTION_HXX
#define NOEDEN_MOVE_ACTION_HXX

#include "action/action.hxx"

/**
 * @brief Action representing the movement of an entity.
 *
 * Action may fail due to blockages, being movement impaired, etc.
 */
class MoveAction : public Action {
public:
    MoveAction();

    ActionResult perform() override;

    /**
     * @brief Destination this entity will arrive at if the action succeeds.
     */
    WorldPos destination;
};

#endif
