#ifndef JARALYN_GFX_SYSTEM_HXX
#define JARALYN_GFX_SYSTEM_HXX

#include "../game.hxx"
#include "../components/components.hxx"
#include "../env/window.hxx"
#include "../env/clock.hxx"
#include "../env/root_config.hxx"
#include "../event/platform_event.hxx"
#include "../system.hxx"
#include "../hud/hud_system.hxx"
#include "ui_renderer.hxx"
#include "world_renderer.hxx"
#include "renderer.hxx"
#include "display.hxx"

class j_gfx_system : public j_system<j_gfx_system> {
private:
    SDL_GLContext gl_context { nullptr };
    j_window* window_ { nullptr };
    j_root_config cfg_;
    j_fps_provider fps_;

    std::unique_ptr<j_ui_renderer> ui_;
    std::unique_ptr<j_world_renderer> world_;

    j_renderer renderer_;
    j_display display_;

    void on_resize(const j_resize_event&);
    void on_root_config_updated(const j_root_config_updated_event&);

    j_texture load_text_texture(const fs::path&) const;

    void adjust_display();

    void configure(const j_root_config& cfg);
public:
    j_gfx_system(j_window* window);

    /**
     * @brief Destroys the GL context.
     */
    ~j_gfx_system();

    void on_load() override;

    void update(uint32_t delta_time) override;

    j_renderer& renderer();
    j_display& display();
};

#endif
