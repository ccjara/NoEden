#include "inventory.hxx"

void jc_item_container::put(j_item&& item) {
    const auto [it, created] = items_.insert(std::make_pair(item.id(), std::move(item)));

    dispatcher_->trigger<j_item_stored_event>(&it->second);
};

const std::unordered_map<j_id, j_item>& jc_item_container::items() const noexcept {
    return items_;
}
