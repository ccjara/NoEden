#ifndef JARALYN_UNIVERSAL_SCENE_HXX
#define JARALYN_UNIVERSAL_SCENE_HXX

#include "scene_definition.hxx"
#include "system_scene.hxx"

class universal_scene : public scene {
private:
    std::vector<const system_scene*> extensions_;

    const scene_definition def_;
public:
    universal_scene(scene_definition&& def);

    const std::vector<system_id_t>& required_systems() const noexcept;

    void extend(const system_scene* sys_scene);
};

#endif
