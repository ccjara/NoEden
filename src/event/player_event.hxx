#ifndef JARALYN_PLAYER_EVENT_HXX
#define JARALYN_PLAYER_EVENT_HXX

/**
 * @brief Triggered as soon as the player assumes control of a unit
 */
struct j_player_control_event {
    /**
     * @brief The entity that is now currently the player
     */
    entt::entity player;
};

/**
 * @brief Triggered when the current controllable unit moved
 */
struct j_player_moved_event {
    /**
     * @brief The entity that is now currently the player
     */
    entt::entity player;

    /**
     * @brief The position the player is after the move
     */
    j_vec2<uint32_t> position;
};

#endif
