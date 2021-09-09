#ifndef JARALYN_UI_WINDOW_PROXY_HXX
#define JARALYN_UI_WINDOW_PROXY_HXX

#include "ui_node_proxy.hxx"
#include "ui_window.hxx"

class UiWindow_proxy : public UiNodeProxy<UiWindow_proxy, UiWindow> {
public:
    using UiNodeProxy::UiNodeProxy;

    /**
     * @brief Proxies UiWindow::set_title
     */
    void set_title(const char* title) {
        node_->set_title(title);
    }

    static inline void declare(luabridge::Namespace& ns) {
        proxy_extend(
            ns,
            "window",
            [](auto& scope) {
                scope.addFunction("set_title", &UiWindow_proxy::set_title);
            }
        );
    }
};

#endif
