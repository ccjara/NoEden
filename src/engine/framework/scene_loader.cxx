#include "scene_loader.hxx"

scene_loader::scene_loader(scene_resolver* const resolver) :
    resolver_(resolver) {
    assert(resolver_);
}

void scene_loader::load_scene(scene_id_t id) {
    current_scene_ = resolver_->resolve_scene_by_id(id);
}

void scene_loader::load_initial_scene() {
    current_scene_ = resolver_->resolve_initial_scene();
}

const scene& scene_loader::current_scene() const noexcept {
    return *current_scene_;
}

