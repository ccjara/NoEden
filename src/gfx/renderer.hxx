#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "../env/window.hxx"
#include "display.hxx"
#include "texture.hxx"
#include "text_shader.hxx"
#include "fps_provider.hxx"

class j_renderer {
private:
    j_size<uint32_t> view_port_ { 0, 0 };
    j_texture tex_;
    std::unique_ptr<j_text_shader> text_shader_ { nullptr };

    SDL_GLContext gl_context { nullptr };
    GLuint vbo { 0 };
    GLuint vao { 0 };

    size_t last_size_ { 0 };
public:
    j_renderer();
    j_renderer(const j_renderer&) = delete;
    j_renderer(j_renderer&&) = delete;
    j_renderer& operator=(j_renderer&&) = delete;
    const j_renderer& operator=(const j_renderer&) = delete;
    ~j_renderer() noexcept;

    void render(const j_display& display);

    void set_viewport(j_size<uint32_t> size) noexcept;
};

#endif
