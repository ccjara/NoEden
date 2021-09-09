#ifndef JARALYN_UI_NODE_PROXY_INTERFACE_HXX
#define JARALYN_UI_NODE_PROXY_INTERFACE_HXX

class UiNodeProxy_interface {
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
     * @brief Proxies UiNode::move
     */
    virtual void move(u32 x, u32 y) = 0;

    /**
     * @brief Proxies UiNode::resize
     */
    virtual void resize(u32 width, u32 height) = 0;

    /**
     * @brief Proxies UiNode::set_anchor_origin
     *
     * `origin` is compared with the AnchorOrigin as enums are not
     * supported by luabridge.
     */
    virtual void set_anchor_origin(u32 origin) = 0;

    /**
     * @brief Proxies UiNode::show
     */
    virtual void show() = 0;

    /**
     * @brief Proxies UiNode::hide
     */
    virtual void hide() = 0;

    /**
     * @brief Proxies UiNode::type
     */
    virtual UiNodeType type() = 0;

    virtual ~UiNodeProxy_interface() = default;
};

#endif
