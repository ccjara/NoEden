#ifndef JARALYN_WALK_ACTION_HXX
#define JARALYN_WALK_ACTION_HXX

#include "action.hxx"

/**
 * @brief Action representing the movement of an actor.
 * 
 * Action may fail due to blockages, being movement impaired, etc.
 */
struct WalkAction : public Action {
    explicit WalkAction(Vec2<i32> destination);

    bool perform() final override;

    u32 base_cost() const override;

    /**
     * @brief Destination this actor will arrive at if the action succeeds.
     */
    Vec2<i32> destination;
};

#endif
