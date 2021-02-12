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
    auto proxy_ptr { new j_ui_window_proxy(window, this) };
    proxies_by_id_.emplace(id, proxy_ptr);
    return proxy_ptr;
}

void j_ui_proxy::store_dependency(lua_State* state, j_ui_node_proxy_interface* proxy_ptr) {
    assert(state);
    assert(proxy_ptr);
    proxies_by_lua_state_.emplace(state, proxy_ptr);
}

void j_ui_proxy::clear_dependencies_by_state(lua_State* state) {
    if (!state) {
        return;
    }
    auto range { proxies_by_lua_state_.equal_range(state) };
    for (auto it = range.first; it != range.second; ++it) {
        it->second->invalidate_lua_state(state);
    }
    proxies_by_lua_state_.erase(state);
}

void j_ui_proxy::clear() {
    proxies_by_id_.clear();
    proxies_by_lua_state_.clear();
    tree_->reset();
}

std::unordered_map<std::string, std::unique_ptr<j_ui_node_proxy_interface>>& j_ui_proxy::node_proxies() {
    return proxies_by_id_;
}
