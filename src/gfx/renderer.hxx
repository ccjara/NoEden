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
    j_vec2<uint32_t> view_port_;
    uint32_t scaling_ { 1 };
    std::unique_ptr<j_text_shader> text_shader_ { nullptr };

    SDL_GLContext gl_context_ { nullptr };
    GLuint vbo { 0 };
    GLuint vao { 0 };

    size_t last_size_ { 0 };

    void reset() noexcept;
public:
    /**
     * @brief Initializes all renderer resources
     *
     * Loads the text shader used to render everything.
     */
    j_renderer() = default;

    j_renderer(const j_renderer&) = delete;
    j_renderer(j_renderer&&) = delete;
    j_renderer& operator=(j_renderer&&) = delete;
    const j_renderer& operator=(const j_renderer&) = delete;

    ~j_renderer() noexcept;

    void set_context(SDL_GLContext context);

    /**
     * @brief Renders the game based on the current state of the given display.
     */
    void render(const j_display& display);

    /**
     * @brief Sets the viewport.
     *
     * Must be called if the user resized the game window.
     */
    void set_viewport(j_vec2<uint32_t> size) noexcept;

    /**
     * @brief Sets the font texture used to display text.
     */
    void set_font(j_texture&& tex) noexcept;

    /**
     * @brief Sets the font's glyph size
     */
    void set_glyph_size(j_vec2<uint32_t> glyph_size) noexcept;

    /**
     * @brief Sets the render scaling
     */
    void set_scaling(uint32_t scaling) noexcept;
};

#endif
