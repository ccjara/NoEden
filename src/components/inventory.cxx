#include "inventory.hxx"

void jc_item_container::put(j_item&& item) {
    items_.insert(std::make_pair(item.id(), std::move(item)));
};

const std::unordered_map<j_id, j_item>& jc_item_container::items() const noexcept {
    return items_;
}
