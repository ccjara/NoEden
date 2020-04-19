#ifndef JARALYN_SCENE_COMPOSER_HXX
#define JARALYN_SCENE_COMPOSER_HXX

#include "scene_interface.hxx"
#include "null_scene.hxx"
#include "world_scene.hxx"
#include "status_scene.hxx"

class j_scene_composer : public j_scene_writer {
private:
    std::vector<std::unique_ptr<j_base_scene>> scenes_;

    bool stack_update_ { false };
public:
    j_scene_composer();
    j_scene& active();

    void load(j_scene_type type) override;
    void unload(j_id id) override;

    void render(j_display& display);
    void update(j_input_state& input);
};

#endif
