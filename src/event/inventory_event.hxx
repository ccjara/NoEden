#ifndef JARALYN_INVENTORY_EVENT_HXX
#define JARALYN_INVENTORY_EVENT_HXX

class jc_inventory;

/**
 * @brief Triggered when the an inventory was opened for view
 */
struct j_inventory_view_event {
    /**
     * @brief The entity which owns the inventory
     */
    entt::entity owner;

    /**
     * @brief A pointer to the inventory component holding the inventory data
     */
    jc_inventory* inventory { nullptr };

    constexpr j_inventory_view_event(
        entt::entity owner,
        jc_inventory* inventory
    ) : owner(owner), inventory(inventory) {
        assert(this->inventory);
    }
};

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
    jc_inventory* container { nullptr };

    constexpr j_gathering_completed_event(
        entt::entity gatherer,
        entt::entity item,
        jc_inventory* container
    ) : gatherer(gatherer), item(item), container(container) {
        assert(this->container);
    }
};

/**
 * @brief Triggered when an item gets added to any inventory
 */
struct j_item_stored_event {
    /**
     * @brief The item which was stored
     */
    entt::entity item;

    // TODO: add container pointer
};

#endif
