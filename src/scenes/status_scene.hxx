#ifndef JARALYN_STATUS_SCENE
#define JARALYN_STATUS_SCENE

#include "base_scene.hxx"

class j_status_scene : public j_base_scene {
public:
    j_status_scene();

    void update(j_input_state& input) override;
    void render(j_display& display) override;
};

#endif
