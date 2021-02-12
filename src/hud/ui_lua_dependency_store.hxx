#ifndef JARALYN_UI_LUA_DEPENDENCY_STORE
#define JARALYN_UI_LUA_DEPENDENCY_STORE

#include "ui_node_proxy_interface.hxx"

/**
 * TODO: Make generic
 */
class j_ui_lua_dependency_store {
public:
    /**
     * @brief Stores the dependency proxy_ptr -> state
     * 
     * As the lua state is invalidated, the dependents have to be informed
     * that they need to clean up their stored lua refs referencing the state.
     */
    virtual void store_dependency(lua_State* state, j_ui_node_proxy_interface* proxy_ptr) = 0;

    /**
     * @brief Removes all dependencies for the given state
     * 
     * This will be called after all dependents have been notified about
     * the invalidation of a lua state. It is expected that all dependents
     * have freed all leftover resources at this point.
     */
    virtual void clear_dependencies_by_state(lua_State* state) = 0;
};

#endif