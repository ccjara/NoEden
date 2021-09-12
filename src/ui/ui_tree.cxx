#include "ui_tree.hxx"

void UiTree::reset() {
    clear();
    create_root_node();
}

void UiTree::clear() {
    nodes_by_id_.clear();
    nodes_.clear();
}

void UiTree::create_root_node() {
    if (root_) {
        LOG(ERROR) << "Root node already exists";
        return;
    }
    root_ = new UiNode();
    nodes_.emplace_back(root_);
    auto [it, inserted] = nodes_by_id_.emplace(std::string("root"), root_);
    assert(inserted);
    root_->id_ = it->first;
    root_->visible_ = true;
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
