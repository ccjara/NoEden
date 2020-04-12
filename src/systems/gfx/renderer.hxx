#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "../../engine/managers/platform/window.hxx"
#include "texture.hxx"
#include "text_shader.hxx"

namespace {
    struct letter {
        float_t sign;
        rgb<float_t> color;
    };
}

class renderer {
private:
    texture tex_;
    text_shader text_shader_;

    const window* window_ = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint vbo { 0 };
    GLuint vao { 0 };

    int rot = 0;
public:
    renderer();
    ~renderer();

    void bind(const window* w);
    void start_rendering();
    void finish_rendering();
};

#endif
