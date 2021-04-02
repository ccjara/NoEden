#ifndef JARALYN_UI_NODE_PROXY_HXX
#define JARALYN_UI_NODE_PROXY_HXX

#include "ui_node_proxy_interface.hxx"
#include "ui_lua_dependency_store.hxx"
#include "../scripts/script_util.hxx"

/**
 * @brief Lua facing proxy interface for a basic j_ui_node
 */
template<typename proxy, typename node>
class j_ui_node_proxy : public j_ui_node_proxy_interface {
public:
    explicit j_ui_node_proxy(
        node* node,
        j_ui_lua_dependency_store* dependencies_
    ) : node_ { node }, dependencies_ { dependencies_ } {
        assert(node_);
        assert(dependencies_);
    }

    virtual void set_handler(luabridge::LuaRef ref) override {
        if (ref.isFunction()) {
            handler_ = ref;
            dependencies_->store_dependency(ref.state(), this);
        }
    }

    virtual void call_handler() override {
        if (handler_) {
            pcall_into(handler_.value(), static_cast<proxy*>(this));
        }
    }

    virtual void invalidate_lua_state(lua_State* state) override {
        if (handler_ && handler_->state() == state) {
            handler_ = std::nullopt;
            return;
        }
    }

    virtual void move(uint32_t x, uint32_t y) override {
        node_->move({ x, y });
    }

    virtual void resize(uint32_t width, uint32_t height) override {
        node_->resize({ width, height });
    }

    virtual ~j_ui_node_proxy() = default;
protected:
    j_ui_lua_dependency_store* dependencies_ { nullptr };
    std::optional<luabridge::LuaRef> handler_;
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
