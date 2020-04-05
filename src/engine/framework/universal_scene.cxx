#include "universal_scene.hxx"

universal_scene::universal_scene(scene_definition&& def) : def_(std::move(def)) {
}

const std::vector<system_id_t>& universal_scene::required_systems() const noexcept {
    return def_.required_systems;
}

void universal_scene::extend(const system_scene* sys_scene) {
    assert(sys_scene);
    extensions_.push_back(sys_scene);
}
