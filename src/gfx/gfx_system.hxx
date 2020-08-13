#ifndef JARALYN_GFX_SYSTEM_HXX
#define JARALYN_GFX_SYSTEM_HXX

#include "../env/window.hxx"
#include "../env/clock.hxx"
#include "../env/root_config.hxx"
#include "../event/event.hxx"
#include "renderer.hxx"
#include "display.hxx"

/**
 * @brief Facade managing graphics.
 */
class j_gfx_system : public j_event_listener {
private:
    SDL_GLContext gl_context { nullptr };
    const j_window* window_ { nullptr };
    j_root_config cfg_;
    j_fps_provider fps_;

    j_renderer renderer_;
    j_display display_;

    void on_resize(const j_resize_event&);
    void on_root_config_updated(const j_root_config_updated_event&);
    void on_window_created(const j_window_created_event&);

    j_texture load_text_texture(const fs::path&) const;

    void adjust_display();
public:
    /**
     * @brief Destroys the GL context.
     */
    ~j_gfx_system();

    /**
     * @brief Prepares the graphics system for the next frame.
     *
     * Must be called before any rendering operation.
     */
    void prepare();

    /**
     * @brief Presents the currently rendered frame.
     *
     * Must be called after all rendering operations are done.
     */
    void present();

    virtual void attach(entt::dispatcher& dispatcher) noexcept override;

    j_renderer& renderer() noexcept;
    j_display& display() noexcept;
};

#endif
