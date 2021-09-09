#ifndef JARALYN_UI_NODE_PROXY_HXX
#define JARALYN_UI_NODE_PROXY_HXX

#include "ui_node_proxy_interface.hxx"
#include "../scripts/script_util.hxx"

namespace lua_anchor_origin {
    static auto center { static_cast<u32>(AnchorOrigin::center) };
    static auto top { static_cast<u32>(AnchorOrigin::top) };
    static auto top_right { static_cast<u32>(AnchorOrigin::top_right) };
    static auto right { static_cast<u32>(AnchorOrigin::right) };
    static auto bottom_right { static_cast<u32>(AnchorOrigin::bottom_right) };
    static auto bottom { static_cast<u32>(AnchorOrigin::bottom) };
    static auto bottom_left { static_cast<u32>(AnchorOrigin::bottom_left) };
    static auto left { static_cast<u32>(AnchorOrigin::left) };
    static auto top_left { static_cast<u32>(AnchorOrigin::top_left) };
}

/**
 * @brief Lua facing proxy interface for a basic UiNode
 */
template<typename proxy, typename node>
class UiNodeProxy : public UiNodeProxy_interface {
public:
    explicit UiNodeProxy(node* n) : node_{n} {
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

    void move(u32 x, u32 y) override {
        node_->move({x, y});
    }

    void resize(u32 width, u32 height) override {
        node_->resize({width, height});
    }

    void set_anchor_origin(u32 origin) override {
        const auto unsafe_origin { static_cast<AnchorOrigin> (origin) };

        switch (unsafe_origin) {
            case AnchorOrigin::center:
            case AnchorOrigin::top:
            case AnchorOrigin::top_right:
            case AnchorOrigin::right:
            case AnchorOrigin::bottom_right:
            case AnchorOrigin::bottom:
            case AnchorOrigin::bottom_left:
            case AnchorOrigin::left:
            case AnchorOrigin::top_left:
                node_->set_anchor_origin(unsafe_origin);
                return;
            default:
                LOG(ERROR) << "Unknown anchor origin " << origin
                           << ". Please use one of the following: "
                              "anchor_origin.top, anchor_origin.top_right, "
                              "anchor_origin.right, anchor_origin.bottom_right, "
                              "anchor_origin.bottom, anchor_origin.bottom_left, "
                              "anchor_origin.left, anchor_origin.top_left or "
                              "anchor_origin.center.";
        }
    }

    void show() override {
        node_->show();
    }

    void hide() override {
        node_->hide();
    }

    UiNodeType type() {
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
            .addFunction("set_anchor_origin", &proxy::set_anchor_origin)
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
