#ifndef JARALYN_INVENTORY_HXX
#define JARALYN_INVENTORY_HXX

#include "item.hxx"
#include "../event/event.hxx"

class jc_item_container {
protected:
    entt::dispatcher* dispatcher_ { nullptr };
    std::vector<entt::entity> items_;
public:
    jc_item_container(entt::dispatcher* const dispatcher) : dispatcher_(dispatcher) {
        assert(dispatcher_);
    }

    void put(entt::entity);

    const std::vector<entt::entity>& items() const noexcept;
};

#endif
