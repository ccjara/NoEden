#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "../../engine/managers/platform/window.hxx"
#include "text_renderer.hxx"

class renderer {
private:
    const window* window_ = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint vbo { 0 };
    GLuint vao { 0 };
    GLuint shader_program;
public:
    const std::shared_ptr<text_renderer> text;

    renderer();
    ~renderer();

    void bind(const window* w);
    void start_rendering();
    void finish_rendering();
};

#endif
