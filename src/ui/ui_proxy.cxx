#include "ui_proxy.hxx"

UiProxy::UiProxy(UiTree* tree) : tree_(tree) {
    assert(tree_);
}

UiWindow_proxy* UiProxy::create_window(const char* id) {
    auto proxy_iter { proxies_by_id_.find(id) };
    if (proxy_iter != proxies_by_id_.end()) {
        return nullptr;
    }
    auto window { tree_->create_node<UiWindow>(nullptr, id) };
    if (!window) {
        return nullptr;
    }
    auto proxy_ptr { new UiWindow_proxy(window) };
    proxies_by_id_.emplace(id, proxy_ptr);
    return proxy_ptr;
}


void UiProxy::reset() {
    proxies_by_id_.clear();
}

std::unordered_map<std::string, std::unique_ptr<UiNodeProxy_interface>>& UiProxy::node_proxies() {
    return proxies_by_id_;
}
