#ifndef JARALYN_UI_PROXY_HXX
#define JARALYN_UI_PROXY_HXX

#include "../hud/ui_tree.hxx"
#include "ui_window_proxy.hxx"
#include "ui_lua_dependency_store.hxx"

/**
 * @brief Lua facing proxy for j_ui_tree
 */
class j_ui_proxy : public j_ui_lua_dependency_store {
public:
    j_ui_proxy(j_ui_tree* tree);

    j_ui_window_proxy* create_window(const char* id);

    static inline void declare(luabridge::Namespace& ns) {
        ns.beginClass<j_ui_proxy>("ui")
        .addFunction("create_window", &j_ui_proxy::create_window)
        .endClass();
    }

    std::unordered_map<std::string, std::unique_ptr<j_ui_node_proxy_interface>>& node_proxies();

    void store_dependency(lua_State* state, j_ui_node_proxy_interface* proxy_ptr) override;
    void clear_dependencies_by_state(lua_State* state) override;
    void clear();
private:
    std::unordered_multimap<lua_State*, j_ui_node_proxy_interface*> proxies_by_lua_state_;
    std::unordered_map<std::string, std::unique_ptr<j_ui_node_proxy_interface>> proxies_by_id_;
    j_ui_tree* tree_ { nullptr };
};

#endif
