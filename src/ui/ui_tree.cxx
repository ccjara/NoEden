#include "ui_tree.hxx"

void UiTree::reset() {
    clear();
    create_root_node();
}

void UiTree::clear() {
    nodes_by_id_.clear();
    nodes_.clear();
    root_ = nullptr;
}

void UiTree::create_root_node() {
    if (root_) {
        Log::error("Root node already exists");
        return;
    }
    root_ = new UiNode();
    nodes_.emplace_back(root_);
    auto [it, inserted] = nodes_by_id_.emplace(std::string("root"), root_);
    assert(inserted);
    root_->id_ = it->first;
    root_->visible_ = true;
}

void UiTree::remove_node(std::string_view id) {
    if (!root_) {
        return;
    }
    auto iter = nodes_by_id_.find(std::string(id));
    if (iter == nodes_by_id_.end()) {
        return;
    }
    if (iter->second->is_root()) {
        return;
    }
    destroy_node(iter->second);

    for (auto& node : nodes_) {
        if (node->destroyed_) {
            nodes_by_id_.erase(node->id_);
        } else {
            // remove destroyed anchors from non-destroyed node
            node->anchored_by_.erase(
                std::remove_if(
                    node->anchored_by_.begin(),
                    node->anchored_by_.end(),
                    [](const UiNode* node) { return node->destroyed_; }
                ),
                node->anchored_by_.end()
            );
            if (node->anchored_to_ && node->anchored_to_->destroyed_) {
                node->anchor_to(*root_);
            }
        }
    }
    // remove all destroyed nodes in owning container
    nodes_.erase(
        std::remove_if(
            nodes_.begin(),
            nodes_.end(),
            [](auto& node) { return node->destroyed_; }
        ),
        nodes_.end()
    );
}

void UiTree::destroy_node(UiNode* node) {
    if (node == nullptr) {
        return;
    }
    node->destroyed_ = true;
    for (UiNode* child : node->children_) {
        destroy_node(child);
    }
}

UiNode* UiTree::get_node_by_id(std::string_view id) {
    auto node_iter { nodes_by_id_.find(std::string(id)) };
    if (node_iter == nodes_by_id_.end()) {
        return nullptr;
    }
    return node_iter->second;
}

UiNode* UiTree::root() const {
    return root_;
}

UiTree::NodeContainer& UiTree::nodes() {
    return nodes_;
}
