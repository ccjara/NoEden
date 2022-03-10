#include "scene_xray.hxx"

SceneXray::SceneXray(EventManager& dispatcher) : events_ { dispatcher } {
    events_.on<SceneLoadedEvent>(this, &SceneXray::on_scene_loaded);
}

bool SceneXray::on_scene_loaded(SceneLoadedEvent& e) {
    scene_ = e.scene;
    return false;
}

void SceneXray::update() {
    if (!scene_) {
        return;
    }
    ImGui::Begin("Scene");

    for (auto& actor : scene_->actors()) {
        ImGui::Text("[%i] %s", actor->id, actor->archetype->name.c_str());
    }

    ImGui::End();
}
