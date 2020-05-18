#ifndef JARALYN_GFX_SYSTEM_HXX
#define JARALYN_GFX_SYSTEM_HXX

#include "../env/window.hxx"
#include "../env/clock.hxx"
#include "renderer.hxx"
#include "display.hxx"

class j_gfx_system {
private:
    SDL_GLContext gl_context { nullptr };
    const j_window* window_ { nullptr };
    j_fps_provider fps_;

    std::unique_ptr<j_renderer> renderer_ = std::make_unique<j_renderer>();

    j_display display_;
public:
    explicit j_gfx_system(const j_window* w);

    ~j_gfx_system();

    void prepare();
    void present();

    j_renderer& renderer() noexcept;
    j_display& display() noexcept;
};

#endif
