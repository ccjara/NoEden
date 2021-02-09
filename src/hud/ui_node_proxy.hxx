#ifndef JARALYN_UI_NODE_PROXY_HXX
#define JARALYN_UI_NODE_PROXY_HXX

#include "ui_node_proxy_interface.hxx"

/**
 * @brief Lua facing proxy interface for a basic j_ui_node
 */
template<typename proxy, typename node>
class j_ui_node_proxy : public j_ui_node_proxy_interface {
public:
    explicit j_ui_node_proxy(node* node) : node_ { node } {
        assert(node_);
    }

    virtual void set_handler(luabridge::LuaRef ref) override {
        if (ref.isFunction()) {
            handler_ = ref;
        }
    }

    virtual void call_handler() override {
        if (handler_.isFunction()) {
            handler_(static_cast<proxy*>(this));
        }
    }

    virtual void move(uint32_t x, uint32_t y) override {
        node_->move({ x, y });
    }

    virtual void resize(uint32_t width, uint32_t height) override {
        node_->resize({ width, height });
    }

    luabridge::LuaRef& handler() { return handler_; };

    virtual ~j_ui_node_proxy() {
        handler_ = nullptr;
    };
protected:
    luabridge::LuaRef handler_ { nullptr };
    node* node_ { nullptr };

    template<typename extension>
    static inline void proxy_extend(
        luabridge::Namespace& ns,
        const char* name,
        extension ext
    ) {
        auto scope = ns.beginClass<proxy>(name)
            .addFunction("set_handler", &proxy::set_handler)
            .addFunction("move", &proxy::move)
            .addFunction("resize", &proxy::resize);

        ext(scope);

        scope.endClass();
    }
};


#endif
