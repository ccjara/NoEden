#ifndef JARALYN_GAME_EVENT_HXX
#define JARALYN_GAME_EVENT_HXX

#include "components/inventory.hxx"
#include "scripts/script.hxx"

enum class j_game_event_type {
    inventory_item_added,
};

/**
 * @brief Maps strings to j_game_event_type
 *
 * Used when interfacing lua and C++. The string representation should
 * only be used in lua and the enum should be used in C++ at all times.
 */
static const std::unordered_map<std::string_view, j_game_event_type> event_type_by_string = {
    { "INVENTORY_ITEM_ADDED", j_game_event_type::inventory_item_added },
};

class j_inventory_item_added_event {
private:
    j_item* item_ { nullptr };
public:
    constexpr j_inventory_item_added_event(j_item* item) :
        item_ { item } {
        assert(item_);
    }

    [[nodiscard]] constexpr const j_item* item() const noexcept {
        return item_;
    }
};

#endif