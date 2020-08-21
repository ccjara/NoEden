#include "inventory.hxx"

void jc_item_container::put(j_item&& item) {
    items_.push_back(std::move(item));
    auto stored_item { &items_.back() };
    items_by_id_.insert(std::make_pair(item.id(), stored_item));
    dispatcher_->trigger<j_item_stored_event>(stored_item);
};

const std::vector<j_item>& jc_item_container::items() const noexcept {
    return items_;
}
