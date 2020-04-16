#ifndef JARALYN_WORLD_SCENE_HXX
#define JARALYN_WORLD_SCENE_HXX

#include "scene.hxx"
#include "../gfx/gfx_system.hxx"
#include "../components.hxx"

class j_world_scene : public j_scene {
public:
    j_world_scene();

    void update(const j_input_state& input) override;
    void render(j_display& display) override;
};

#endif
