#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "../env/window.hxx"
#include "display.hxx"
#include "texture.hxx"
#include "text_shader.hxx"
#include "fps_provider.hxx"

/**
 * @brief Executes GL rendering operations
 */
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
    /**
     * @brief Initializes all renderer resources
     *
     * Loads the text texture and its managing shader used to render everything.
     */
    j_renderer();

    j_renderer(const j_renderer&) = delete;
    j_renderer(j_renderer&&) = delete;
    j_renderer& operator=(j_renderer&&) = delete;
    const j_renderer& operator=(const j_renderer&) = delete;

    ~j_renderer() noexcept;

    /**
     * @brief Renders the game based on the current state of the given display.
     */
    void render(const j_display& display);

    /**
     * @brief Sets the viewport.
     *
     * Must be called if the user resized the game window.
     */
    void set_viewport(j_size<uint32_t> size) noexcept;
};

#endif
