#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "../../engine/managers/platform/window.hxx"
#include "text_renderer.hxx"
#include "text_shader.hxx"

class renderer {
private:
    text_shader text_shader_;

    const window* window_ = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint vbo { 0 };
    GLuint vao { 0 };
    GLuint ebo { 0 };
public:
    const std::shared_ptr<text_renderer> text;

    renderer();
    ~renderer();

    void bind(const window* w);
    void start_rendering();
    void finish_rendering();
};

#endif
