#ifndef JARALYN_WORLD_EVENT_HXX
#define JARALYN_WORLD_EVENT_HXX

/**
 * @brief Triggered when the world time advances by a specific amount (in steps)
 */
struct j_world_step_event {
    /**
     * @brief The number of steps that the world time has advanced by
     */
    uint32_t steps { 0 };

    explicit constexpr j_world_step_event(uint32_t steps) : steps { steps } {
    }
};

#endif