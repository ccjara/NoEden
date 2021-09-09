#ifndef JARALYN_UI_PROXY_HXX
#define JARALYN_UI_PROXY_HXX

#include "ui_tree.hxx"
#include "ui_window_proxy.hxx"

/**
 * @brief Lua facing proxy for UiTree
 */
class UiProxy {
public:
    explicit UiProxy(UiTree* tree);

    UiWindow_proxy* create_window(const char* id);

    static inline void declare(luabridge::Namespace& ns) {
        ns.beginClass<UiProxy>("ui")
        .addFunction("create_window", &UiProxy::create_window)
        .endClass();
    }

    std::unordered_map<std::string, std::unique_ptr<UiNodeProxy_interface>>& node_proxies();

    void reset();
private:
    std::unordered_map<std::string, std::unique_ptr<UiNodeProxy_interface>> proxies_by_id_;
    UiTree* tree_ { nullptr };
};

#endif
