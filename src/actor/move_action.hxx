#ifndef JARALYN_MOVE_ACTION_HXX
#define JARALYN_MOVE_ACTION_HXX

#include "action.hxx"
#include "../scene/scene.hxx"

/**
 * @brief Action representing the movement of an actor.
 *
 * Action may fail due to blockages, being movement impaired, etc.
 */
struct MoveAction : public Action {
    explicit MoveAction(Vec2<i32> destination);

    bool perform() final override;

    u32 base_cost() const override;

    /**
     * @brief Destination this actor will arrive at if the action succeeds.
     */
    Vec2<i32> destination;
};

#endif
