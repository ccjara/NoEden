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

    struct TileWindowData {
        bool is_editing = false;
        TileType lmb_type = TileType::None;
        TileType rmb_type = TileType::None;
    } tile_window_data_;

    void entity_window();
    void tile_window();

    bool on_scene_loaded(SceneLoadedEvent& e);
    bool on_mouse_down(MouseDownEvent& e);
};

#endif
