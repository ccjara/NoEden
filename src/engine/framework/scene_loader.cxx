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

    for (const auto id : current_scene_->required_systems()) {
        const auto& sys { platform_->load_system(id) };

        current_scene_->extend(&sys.scene());
    }
}

void scene_loader::load_initial_scene() {
    return load_scene(factory_->get_initial_scene_id());
}

const universal_scene& scene_loader::current_scene() const noexcept {
    return *current_scene_;
}
