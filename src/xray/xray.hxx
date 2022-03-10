#ifndef JARALYN_XRAY_HXX
#define JARALYN_XRAY_HXX

#include "../gfx/gfx_event.hxx"
#include "../game/window.hxx"
#include "../input/input_event.hxx"
#include "xray_interface.hxx"
#include "xray_event.hxx"

class Xray {
public:
    Xray(Window& window, EventManager& events);

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
    Window& window_;
    EventManager& events_;

    bool on_post_render(PostRenderEvent& e);
    bool on_mouse_down(MouseDownEvent& e);
    bool on_mouse_up(MouseUpEvent& e);
    bool on_key_down(KeyDownEvent& e);
    bool on_key_up(KeyUpEvent& e);

    ImGuiContext* imgui_context_ { nullptr };

    std::vector<std::unique_ptr<IXray>> xrays_;
};

#endif
