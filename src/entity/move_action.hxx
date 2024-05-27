#ifndef JARALYN_MOVE_ACTION_HXX
#define JARALYN_MOVE_ACTION_HXX

#include "action.hxx"

/**
 * @brief Action representing the movement of an entity.
 *
 * Action may fail due to blockages, being movement impaired, etc.
 */
struct MoveAction : public Action {
    explicit MoveAction(Vec2<i32> destination);

    bool perform() final override;

    u32 base_cost() const override;

    /**
     * @brief Destination this entity will arrive at if the action succeeds.
     */
    Vec2<i32> destination;

    // TODO: scriptable / configurable
    const static inline u32 BASE_COST = 100;
};

#endif
