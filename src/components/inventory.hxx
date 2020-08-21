#ifndef JARALYN_INVENTORY_HXX
#define JARALYN_INVENTORY_HXX

#include "item.hxx"
#include "../event/event.hxx"

class jc_item_container {
protected:
    entt::dispatcher* dispatcher_ { nullptr };
    std::vector<j_item> items_;
    std::unordered_map<j_id, j_item*> items_by_id_;
public:
    jc_item_container(entt::dispatcher* const dispatcher) : dispatcher_(dispatcher) {
        assert(dispatcher_);
    }

    void put(j_item&& item);

    const std::vector<j_item>& items() const noexcept;
};

#endif
