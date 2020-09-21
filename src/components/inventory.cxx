#include "inventory.hxx"

void jc_item_container::put(entt::entity entity) {
    items_.emplace_back(entity);
    dispatcher_->trigger<j_item_stored_event>(entity);
};

const std::vector<entt::entity>& jc_item_container::items() const {
    return items_;
}
