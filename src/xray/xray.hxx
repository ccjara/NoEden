#ifndef JARALYN_XRAY_HXX
#define JARALYN_XRAY_HXX

#include "../gfx/gfx_event.hxx"
#include "../game/window.hxx"
#include "xray_interface.hxx"

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

    void start(SDL_GLContext context);
    void stop();

    void update();
private:
    Window& window_;
    entt::dispatcher& dispatcher_;

    void on_post_render(const PostRenderEvent& e);

    ImGuiContext* imgui_context_ { nullptr };

    std::vector<std::unique_ptr<IXray>> xrays_;
};

#endif
