#ifndef JARALYN_GAME_EVENT_HXX
#define JARALYN_GAME_EVENT_HXX

#include "components/inventory.hxx"

class j_inventory_item_added_event {
private:
    j_item* item_ { nullptr };
public:
    j_inventory_item_added_event(j_item* item): 
        item_ { item } {
    }

    [[nodiscard]] const j_item* item() const noexcept {
        return item_;
    }
};

#endif