#include "scene_loader.hxx"

scene_loader::scene_loader(
    scene_factory* const factory,
    platform_manager* const platform
) : factory_(factory), platform_(platform) {
    assert(factory_);
    assert(platform_);
}

void scene_loader::load_scene(scene_id_t id) {
    current_scene_ = factory_->create(id);

    for (const auto id : current_scene_->list_required_systems()) {
        platform_->load_system(id);
    }
}

void scene_loader::load_initial_scene() {
    return load_scene(factory_->get_initial_scene_id());
}

const scene& scene_loader::current_scene() const noexcept {
    return *current_scene_;
}

