#include "../system.hxx"
#include "../env/window.hxx"
#include "renderer.hxx"
#include "display.hxx"

class j_gfx_system : public j_system {
private:
    SDL_GLContext gl_context { nullptr };
    const j_window* window_ { nullptr };

    std::unique_ptr<j_renderer> renderer_ = std::make_unique<j_renderer>();

    j_display display_;
public:
    explicit j_gfx_system(const j_window* w);

    ~j_gfx_system();

    void temp__render();
};
