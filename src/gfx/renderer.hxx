#ifndef NOEDEN_RENDERER_HXX
#define NOEDEN_RENDERER_HXX

#include "config/config.hxx"
#include "config/config_event.hxx"
#include "gfx/display.hxx"
#include "gfx/shader.hxx"
#include "gfx/texture.hxx"
#include "platform/platform_event.hxx"

#include <resource/resource_manager.hxx>

class ResourceManager;

/**
 * @brief Executes GL rendering operations
 */
class Renderer {
public:
    explicit Renderer(EventManager* events, ResourceManager* res);

    /**
     * @brief Initializes renderer resources
     */
    bool initialize();

    /**
     * @brief Renders the game based on the current display state.
     */
    void render();

    /**
     * @brief Sets the viewport.
     *
     * Must be called if the user resized the game window.
     */
    void set_viewport(glm::ivec2 size);

    /**
     * @brief Sets the font's glyph size
     */
    void set_glyph_size(glm::ivec2 glyph_size);

    /**
     * @brief Sets the render scaling
     */
    void set_scaling(i32 scaling);

    /**
     * @brief Returns the result of dividing the glyph width by its height
     */
    float glyph_aspect_ratio() const;

    /**
     * @brief Returns the current text texture GL id
     */
    GLuint text_texture() const;

    /**
     * @brief Calculates texture coordinates of a glyph for a custom render
     *
     * The array will contain [u1, v1, u2, v2] in this order.
     */
    std::array<f32, 4> calculate_glyph_uv(u32 glyph) const;

    /**
     * @brief Provides writable access to the Display
     */
    Display& display();

    Display& ui_layer();

    ~Renderer();

private:
    Config cfg_;
    Texture text_texture_;

    glm::ivec2 view_port_ = { 0, 0 };
    glm::ivec2 glyph_size_ = { 0, 0 };
    i32 scaling_ = 1;

    Shader* shader_ = nullptr;

    GLuint vbo = 0;
    GLuint vao = 0;

    size_t last_size_ = 0;

    EventResult on_resize(const WindowResizedEvent&);
    Subscription<WindowResizedEvent> resize_sub_;
    EventResult on_config_updated(const ConfigUpdatedEvent&);
    Subscription<ConfigUpdatedEvent> config_updated_sub_;

    void adjust_display();

    bool configure(const Config& cfg);

    std::array<Display, 2> layers_;

    EventManager* events_ = nullptr;
    ResourceManager* res_ = nullptr;

    bool initialized_ = false;
};

#endif
