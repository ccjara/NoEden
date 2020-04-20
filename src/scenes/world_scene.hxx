#ifndef JARALYN_WORLD_SCENE_HXX
#define JARALYN_WORLD_SCENE_HXX

#include "base_scene.hxx"
#include "../components.hxx"
#include "status_scene.hxx"

class j_world_scene : public j_base_scene {
protected:
    entt::entity player_;
public:
    j_world_scene();

    void update(j_input_state& input) override;
    void render(j_display& display) override;
};

#endif
