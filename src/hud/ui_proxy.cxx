#include "ui_proxy.hxx"

j_ui_proxy::j_ui_proxy(j_ui_tree* tree) : tree_(tree) {
    assert(tree_);
}

j_ui_window_proxy* j_ui_proxy::create_window(const char* id) {
    auto proxy_iter { proxies_by_id_.find(id) };
    if (proxy_iter != proxies_by_id_.end()) {
        return nullptr;
    }
    auto window { tree_->create_node<j_ui_window>(nullptr, id) };
    if (!window) {
        return nullptr;
    }
    auto proxy_ptr { new j_ui_window_proxy(window) };
    proxies_by_id_.emplace(id, proxy_ptr);
    return proxy_ptr;
}


void j_ui_proxy::reset() {
    proxies_by_id_.clear();
}

std::unordered_map<std::string, std::unique_ptr<j_ui_node_proxy_interface>>& j_ui_proxy::node_proxies() {
    return proxies_by_id_;
}
