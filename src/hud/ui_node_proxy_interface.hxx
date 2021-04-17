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

    /**
     * @brief Proxies j_ui_node::set_anchor_origin
     *
     * `origin` is compared with the j_ui_anchor_origin as enums are not
     * supported by luabridge.
     */
    virtual void set_anchor_origin(uint32_t origin) = 0;

    /**
     * @brief Proxies j_ui_node::show
     */
    virtual void show() = 0;

    /**
     * @brief Proxies j_ui_node::hide
     */
    virtual void hide() = 0;

    /**
     * @brief Proxies j_ui_node::type
     */
    virtual j_ui_node_type type() = 0;

    virtual ~j_ui_node_proxy_interface() = default;
};

#endif
