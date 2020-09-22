#include "item.hxx"

void jc_inventory::put(uint32_t container_index, entt::entity item) {
    if (container_index >= containers_.size()) {
        LOG(ERROR) << "Cannot put item "
                   << static_cast<uint32_t> (item)
                   << "into container #"
                   << container_index << ": container does not exist";
        return;
    }
    containers_[container_index].store(item);
}

j_item_container* jc_inventory::create_container() {
    return &containers_.emplace_back();
}

j_item_container* jc_inventory::get_container(uint32_t index) {
    if (index >= containers_.size()) {
        LOG(ERROR) << "Container #" << index << " does not exist";
        return nullptr;
    }
    return &containers_[index];
}

void j_item_container::store(entt::entity item) {
    items_.emplace_back(item);
}

const std::vector<entt::entity>& j_item_container::items() const {
    return items_;
}
