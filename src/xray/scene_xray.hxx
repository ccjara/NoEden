#ifndef NOEDEN_SCENE_XRAY_HXX
#define NOEDEN_SCENE_XRAY_HXX

#include "xray/xray.hxx"
#include "config/config_event.hxx"
#include "config/config.hxx"
#include "realm/realm_event.hxx"
#include "xray/noise_texture.hxx"

class Entity;
struct WorldContext;

class SceneXray : public Xray {
public:
    void render() override;

    bool initialize() override;
private:
    void entity_panel(std::optional<Id> entity_id, WorldContext& world_context);
    void entity_glyph(Entity* entity);

    void entity_window(WorldContext& world_context);
    void tile_window(WorldContext& world_context);
    void mapgen_window(WorldContext& world_context);

    EventResult on_mouse_down(const MouseButtonDownEvent& e);
    Subscription<MouseButtonDownEvent> mouse_down_sub_;
    EventResult on_config_updated(const ConfigUpdatedEvent& e);
    Subscription<ConfigUpdatedEvent> config_updated_sub_;
    EventResult on_realm_loaded(const RealmLoadedEvent& e);
    Subscription<RealmLoadedEvent> realm_loaded_sub_;

    Config config_;
    NoiseTexture noise_texture_;
    WorldContext* world_context_ = nullptr;
};

#endif
