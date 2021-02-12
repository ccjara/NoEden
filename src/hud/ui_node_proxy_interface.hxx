#ifndef JARALYN_UI_NODE_PROXY_INTERFACE_HXX
#define JARALYN_UI_NODE_PROXY_INTERFACE_HXX

class j_ui_node_proxy_interface {
public:
    /**
     * @brief Assigns a callback (from lua code)
     * 
     * This callback is invoked by the engine during the update phase of the script system
     * 
     * @see call_handler()
     */
    virtual void set_handler(luabridge::LuaRef ref) = 0;

    /**
     * @brief Orders the node to cleanup all lua resources belonging to the given state
     */
    virtual void invalidate_lua_state(lua_State* state) = 0;

    /**
     * @brief Invokes the handler assigned from lua code
     * 
     * This method is called by the script system. If no handler is assigned
     * by a lua script this will be a no op.
     */
    virtual void call_handler() = 0;

    /**
     * @brief Proxies j_ui_node::move
     */
    virtual void move(uint32_t x, uint32_t y) = 0;

    /**
     * @brief Proxies j_ui_node::resize
     */
    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual ~j_ui_node_proxy_interface() = default;
};

#endif
