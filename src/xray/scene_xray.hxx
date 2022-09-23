#ifndef JARALYN_SCENE_XRAY_HXX
#define JARALYN_SCENE_XRAY_HXX

#include "xray_interface.hxx"
#include "../input/input.hxx"
#include "../input/input_event.hxx"
#include "../scene/scene.hxx"
#include "../gfx/renderer.hxx"

class SceneXray : public IXray {
public:
    SceneXray();

    void update() override;
private:
    void actor_panel(std::optional<u64> actor_id);
    void actor_glyph(Actor* actor);

    struct TileWindowData {
        bool is_editing = false;
        TileType lmb_type = TileType::None;
        TileType rmb_type = TileType::None;
    } tile_window_data_;

    void entity_window();
    void tile_window();

    bool on_mouse_down(MouseDownEvent& e);
};

#endif
