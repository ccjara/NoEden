#ifndef JARALYN_GFX_SYSTEM_HXX
#define JARALYN_GFX_SYSTEM_HXX

#include "../env/window.hxx"
#include "../env/clock.hxx"
#include "../env/env_interface.hxx"
#include "renderer.hxx"
#include "display.hxx"

class j_gfx_system : public j_env_event_listener {
private:
    SDL_GLContext gl_context { nullptr };
    const j_window* window_ { nullptr };
    j_fps_provider fps_;

    std::unique_ptr<j_renderer> renderer_;

    j_display display_;

    void on_resize_event(const j_resize_event&);
public:
    explicit j_gfx_system(const j_window* w);
    ~j_gfx_system();

    void prepare();
    void present();

    virtual void attach(entt::dispatcher& dispatcher) override;

    j_renderer& renderer() noexcept;
    j_display& display() noexcept;
};

#endif
