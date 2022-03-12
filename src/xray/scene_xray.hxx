#ifndef JARALYN_SCENE_XRAY_HXX
#define JARALYN_SCENE_XRAY_HXX

#include "xray_interface.hxx"
#include "../scene/scene.hxx"
#include "../scene/scene_events.hxx"
#include "../actor/player_controller.hxx"
#include "../gfx/renderer.hxx"

class SceneXray : public IXray {
public:
    explicit SceneXray(
        EventManager& dispatcher,
        PlayerController& playerController,
        Renderer& renderer
    );

    void update() override;
private:
    EventManager& events_;
    PlayerController& player_controller_;
    Renderer& renderer_;
    Scene* scene_ { nullptr };

    void actor_panel(std::optional<u64> actor_id);
    void actor_glyph(Actor* actor);

    bool on_scene_loaded(SceneLoadedEvent& e);
};

#endif
