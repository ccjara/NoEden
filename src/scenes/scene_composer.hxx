#ifndef JARALYN_SCENE_COMPOSER_HXX
#define JARALYN_SCENE_COMPOSER_HXX

#include "scene.hxx"
#include "world_scene.hxx"

class j_scene_composer {
private:
    std::stack<std::unique_ptr<j_scene>> scene_stack_;
public:
    template<class scene_t>
    void load();
};

template<>
void j_scene_composer::load<j_world_scene>() {

}

#endif