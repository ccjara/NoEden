#ifndef JARALYN_GATHERING_EVENT_HXX
#define JARALYN_GATHERING_EVENT_HXX

class jc_inventory;

/**
 * @brief Triggered when any unit started a gathering attempt
 */
struct j_gathering_started_event {
    /**
     * @brief The entity which started the gathering attempt
     */
    entt::entity gatherer;

    /**
     * @brief Snapshot position where the gathering attempt takes place
     */
    j_vec2<uint32_t> position;
};

/**
 * @brief Triggered when any unit successfully completed a gathering attempt
 */
struct j_gathering_completed_event {
    /**
     * @brief The entity which completed the gathering attempt
     */
    entt::entity gatherer;

    /**
     * @brief The item which was picked up
     */
    entt::entity item;

    /**
     * @brief The container which now contains the picked up item
     */
    j_item_container* container { nullptr };

    constexpr j_gathering_completed_event(
        entt::entity gatherer,
        entt::entity item,
        j_item_container* container
    ) : gatherer(gatherer), item(item), container(container) {
        assert(this->container);
    }
};

#endif
