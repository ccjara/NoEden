#ifndef JARALYN_UI_NODE_PROXY_HXX
#define JARALYN_UI_NODE_PROXY_HXX

#include "ui_node_proxy_interface.hxx"
#include "../scripts/script_util.hxx"

/**
 * @brief Lua facing proxy interface for a basic j_ui_node
 */
template<typename proxy, typename node>
class j_ui_node_proxy : public j_ui_node_proxy_interface {
public:
    explicit j_ui_node_proxy(node* n) : node_ { n } {
        assert(node_);
    }

    void set_handler(luabridge::LuaRef ref) override {
        if (ref.isFunction()) {
            handler_ = ref;
        }
    }

    void call_handler() override {
        if (handler_) {
            pcall_into(handler_.value(), static_cast<proxy*>(this));
        }
    }

    void move(uint32_t x, uint32_t y) override {
        node_->move({ x, y });
    }

    void resize(uint32_t width, uint32_t height) override {
        node_->resize({ width, height });
    }

    void show() override {
        node_->show();
    }

    void hide() override {
        node_->hide();
    }

    j_ui_node_type type() {
        return node_->type();
    }
protected:
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
            .addFunction("resize", &proxy::resize)
            .addFunction("show", &proxy::show)
            .addFunction("hide", &proxy::hide);

        ext(scope);

        scope.endClass();
    }
};


#endif
