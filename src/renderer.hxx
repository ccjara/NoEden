#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "window.hxx"
#include "text_renderer.hxx"

class renderer {
private:
    SDL_GLContext gl_context;
    std::shared_ptr<window> wnd;
public:
    const std::shared_ptr<text_renderer> text;

    renderer();
    ~renderer();

    void bind(std::shared_ptr<window> w);
    void start_rendering();
    void finish_rendering();

    void project();
};

#endif
