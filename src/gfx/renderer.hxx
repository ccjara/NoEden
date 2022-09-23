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
    Renderer() = delete;

    static void init();
    static void shutdown();

    /**
     * @brief Renders the game based on the current display state.
     */
    static void render();

    /**
     * @brief Sets the viewport.
     *
     * Must be called if the user resized the game window.
     */
    static void set_viewport(Vec2<u32> size);

    /**
     * @brief Sets the font texture used to display text.
     */
    static void set_font(Texture* tex);

    /**
     * @brief Sets the font's glyph size
     */
    static void set_glyph_size(Vec2<u32> glyph_size);

    /**
     * @brief Sets the render scaling
     */
    static void set_scaling(u32 scaling);

    /**
     * @brief Returns the result of dividing the glyph width by its height
     */
    static [[nodiscard]] float glyph_aspect_ratio();

    /**
     * @brief Returns the current gl context
     */
    static [[nodiscard]] SDL_GLContext gl_context();

    /**
     * @brief Returns the current text texture GL id
     */
    static [[nodiscard]] GLuint text_texture();

    /**
     * @brief Calculates texture coordinates of a glyph for a custom render
     *
     * The array will contain [u1, v1, u2, v2] in this order.
     */
    static [[nodiscard]] std::array<float, 4> calculate_glyph_uv(u32 glyph);

    /**
     * @brief Provides writable access to the Display
     */
    static [[nodiscard]] Display& display();
private:
    static inline SDL_GLContext gl_context_ = nullptr;
    static inline Config cfg_;
    static inline Texture text_texture_;
    static inline Display display_;

    static inline Vec2<u32> view_port_;
    static inline u32 scaling_ = 1;
    static inline std::unique_ptr<TextShader> text_shader_ = nullptr;

    static inline GLuint vbo = 0;
    static inline GLuint vao = 0;

    static inline size_t last_size_ = 0;

    static inline void update_display();

    static inline bool on_resize(ResizeEvent&);
    static inline bool on_config_updated(ConfigUpdatedEvent&);

    static inline void load_text_texture(const fs::path&);

    static inline void adjust_display();
    static inline void render_entities();

    static inline void configure(const Config& cfg);
};

#endif
