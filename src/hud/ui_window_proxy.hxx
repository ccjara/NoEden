#ifndef JARALYN_UI_WINDOW_PROXY_HXX
#define JARALYN_UI_WINDOW_PROXY_HXX

#include "ui_node_proxy.hxx"
#include "../hud/ui_window.hxx"

class j_ui_window_proxy : public j_ui_node_proxy<j_ui_window_proxy, j_ui_window> {
public:
    using j_ui_node_proxy::j_ui_node_proxy;

    /**
     * @brief Proxies j_ui_window::set_title
     */
    void set_title(const char* title) {
        node_->set_title(title);
    }

    static inline void declare(luabridge::Namespace& ns) {
        proxy_extend(
            ns,
            "window",
            [](auto& scope) {
                scope.addFunction("set_title", &j_ui_window_proxy::set_title);
            }
        );
    }
};

#endif
