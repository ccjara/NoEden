#include "scene_composer.hxx"

j_scene_composer::j_scene_composer() {
    scenes_.emplace_back(new j_null_scene());
}

j_scene& j_scene_composer::active() {
    return *scenes_.back();
}

j_scene* j_scene_composer::load(j_scene_type type) {
    std::unique_ptr<j_base_scene> scene_ptr { nullptr };

    switch (type) {
    case j_scene_type::status:
        scene_ptr = std::make_unique<j_status_scene>();
        break;
    case j_scene_type::world:
        scene_ptr = std::make_unique<j_world_scene>();
        break;
    default:
        LOG(ERROR) << "Unknown scene type " << static_cast<int32_t>(type);
        throw;
    }
    scene_ptr->attach(this);

    auto raw_ptr { scene_ptr.get() };

    scenes_.push_back(std::move(scene_ptr));

    stack_update_ = true;

    return raw_ptr;
}

void j_scene_composer::unload(j_id id) {
    const auto scene_it = std::find_if(
        scenes_.cbegin(),
        scenes_.cend(),
        [id](const auto& scene) {
            return scene->id() == id;
        }
    );
    if (scene_it == scenes_.end()) {
        LOG(ERROR) << "Scene #" << id << " not found";
        return;
    }
    scenes_.erase(scene_it);

    stack_update_ = true;
}

void j_scene_composer::render(j_display& display) {
    for (auto& it { scenes_.rbegin() }; it != scenes_.rend(); it++) {
        auto& scene { *it->get() };

        scene.render(display);

        if (stack_update_ || scene.opaque()) {
            return;
        }
    }
}

void j_scene_composer::update(j_input_state& input) {
    stack_update_ = false;

    for (auto& it { scenes_.rbegin() }; it != scenes_.rend(); it++) {
        auto& scene { *it->get() };

        scene.update(input);

        if (stack_update_ || scene.blocking()) {
            return;
        }
    }
}