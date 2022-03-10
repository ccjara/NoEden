#ifndef JARALYN_RENDERER_HXX
#define JARALYN_RENDERER_HXX

#include "display.hxx"
#include "texture.hxx"
#include "text_shader.hxx"
#include "../game/platform_event.hxx"
#include "../game/window.hxx"
#include "../game/config.hxx"
#include "../scene/scene.hxx"
#include "gfx_event.hxx"

/**
 * @brief Executes GL rendering operations
 */
class Renderer {
public:
    Renderer(Window& window, EventManager& dispatcher);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;
    const Renderer& operator=(const Renderer&) = delete;

    /**
     * @brief Starts the underlying graphics api
     */
    void initialize();

    /**
     * @brief Renders the game based on the current display state.
     */
    void render(const Scene& scene);

    /**
     * @brief Sets the viewport.
     *
     * Must be called if the user resized the game window.
     */
    void set_viewport(Vec2<u32> size);

    /**
     * @brief Sets the font texture used to display text.
     */
    void set_font(Texture* tex);

    /**
     * @brief Sets the font's glyph size
     */
    void set_glyph_size(Vec2<u32> glyph_size);

    /**
     * @brief Sets the render scaling
     */
    void set_scaling(u32 scaling);

    /**
     * @brief Returns the current gl context
     */
    SDL_GLContext gl_context() const;
private:
    Window& window_;

    EventManager& events_;
    SDL_GLContext gl_context_ { nullptr };
    Config cfg_;
    Texture text_texture_;
    Display display_;

    Vec2<u32> view_port_;
    u32 scaling_ { 1 };
    std::unique_ptr<TextShader> text_shader_ { nullptr };

    GLuint vbo { 0 };
    GLuint vao { 0 };

    size_t last_size_ { 0 };

    void update_display(const Scene& scene);

    bool on_resize(ResizeEvent&);
    bool on_config_updated(ConfigUpdatedEvent&);

    void load_text_texture(const fs::path&) const;

    void adjust_display();
    void render_entities();

    void configure(const Config& cfg);

    void reset();
};

#endif
