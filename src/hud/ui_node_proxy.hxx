#ifndef JARALYN_UI_NODE_PROXY_HXX
#define JARALYN_UI_NODE_PROXY_HXX

#include "ui_node_proxy_interface.hxx"
#include "../scripts/script_util.hxx"

namespace lua_anchor_origin {
    static auto center { static_cast<uint32_t>(j_ui_anchor_origin::center) };
    static auto top { static_cast<uint32_t>(j_ui_anchor_origin::top) };
    static auto top_right { static_cast<uint32_t>(j_ui_anchor_origin::top_right) };
    static auto right { static_cast<uint32_t>(j_ui_anchor_origin::right) };
    static auto bottom_right { static_cast<uint32_t>(j_ui_anchor_origin::bottom_right) };
    static auto bottom { static_cast<uint32_t>(j_ui_anchor_origin::bottom) };
    static auto bottom_left { static_cast<uint32_t>(j_ui_anchor_origin::bottom_left) };
    static auto left { static_cast<uint32_t>(j_ui_anchor_origin::left) };
    static auto top_left { static_cast<uint32_t>(j_ui_anchor_origin::top_left) };
}

/**
 * @brief Lua facing proxy interface for a basic j_ui_node
 */
template<typename proxy, typename node>
class j_ui_node_proxy : public j_ui_node_proxy_interface {
public:
    explicit j_ui_node_proxy(node* n) : node_{n} {
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
        node_->move({x, y});
    }

    void resize(uint32_t width, uint32_t height) override {
        node_->resize({width, height});
    }

    void set_anchor_origin(uint32_t origin) override {
        const auto unsafe_origin { static_cast<j_ui_anchor_origin> (origin) };

        switch (unsafe_origin) {
            case j_ui_anchor_origin::center:
            case j_ui_anchor_origin::top:
            case j_ui_anchor_origin::top_right:
            case j_ui_anchor_origin::right:
            case j_ui_anchor_origin::bottom_right:
            case j_ui_anchor_origin::bottom:
            case j_ui_anchor_origin::bottom_left:
            case j_ui_anchor_origin::left:
            case j_ui_anchor_origin::top_left:
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
