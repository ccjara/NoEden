#ifndef JARALYN_SCENE_COMPOSER_HXX
#define JARALYN_SCENE_COMPOSER_HXX

#include "scene.hxx"
#include "world_scene.hxx"

class j_scene_composer {
private:
    std::stack<std::unique_ptr<j_scene>> scene_stack_;
public:
    j_scene_composer() {
        scene_stack_.emplace(new j_null_scene());
    }

    template<class scene_t>
    void load();

    j_scene& active() {
        return *scene_stack_.top();
    }
};

template<>
inline void j_scene_composer::load<j_world_scene>() {
    scene_stack_.emplace(new j_world_scene());
}


#endif