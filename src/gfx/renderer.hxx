#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "../env/window.hxx"
#include "display.hxx"
#include "texture.hxx"
#include "text_shader.hxx"
#include "fps_provider.hxx"

class j_renderer {
private:
    j_texture tex_;
    std::unique_ptr<j_text_shader> text_shader_;

    const j_window* window_ = nullptr;
    SDL_GLContext gl_context = nullptr;
    GLuint vbo { 0 };
    GLuint vao { 0 };

    size_t last_size_ { 0 };
public:
    j_renderer() = default;
    j_renderer(const j_renderer&) = delete;
    j_renderer(j_renderer&&) = delete;
    j_renderer& operator=(j_renderer&&) = delete;
    const j_renderer& operator=(const j_renderer&) = delete;
    ~j_renderer();

    void bind(const j_window* window);
    void render(const j_display& display);
};

#endif
