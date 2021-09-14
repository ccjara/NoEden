#ifndef JARALYN_XRAY_HXX
#define JARALYN_XRAY_HXX

#include "../gfx/gfx_event.hxx"
#include "../game/window.hxx"
#include "xray_interface.hxx"
#include "xray_event.hxx"

class Xray {
public:
    Xray(Window& window, entt::dispatcher& dispatcher);

    /**
     * @brief Adds an xray implementation to the xray container
     */
    template<typename Xray, typename... XrayArgs>
    void add(XrayArgs&&... xray_args) {
        xrays_.emplace_back(new Xray(std::forward<XrayArgs>(xray_args)...));
    }

    void startup(SDL_GLContext context);
    void shutdown();
private:
    bool focused_ { false };

    Window& window_;
    entt::dispatcher& dispatcher_;

    void on_post_render(const PostRenderEvent& e);

    ImGuiContext* imgui_context_ { nullptr };

    std::vector<std::unique_ptr<IXray>> xrays_;
};

#endif
