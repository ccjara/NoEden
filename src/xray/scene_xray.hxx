#ifndef NOEDEN_SCENE_XRAY_HXX
#define NOEDEN_SCENE_XRAY_HXX

#include "xray/xray.hxx"
#include "config/config_event.hxx"
#include "config/config.hxx"
#include "xray/noise_texture.hxx"

class Entity;

class SceneXray : public Xray {
public:
    void render() override;

    bool initialize() override;
private:
    void entity_panel(std::optional<Id> entity_id);
    void entity_glyph(Entity* entity);

    void entity_window();
    void tile_window();
    void mapgen_window();

    // TODO: inject service locator instead (by xray_manager), add initialize() method

    EventResult on_mouse_down(const MouseDownEvent& e);
    Subscription<MouseDownEvent> mouse_down_sub_;
    EventResult on_config_updated(const ConfigUpdatedEvent& e);
    Subscription<ConfigUpdatedEvent> config_updated_sub_;

    Config config_;
    NoiseTexture noise_texture_;
};

#endif
