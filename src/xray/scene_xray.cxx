#include "scene_xray.hxx"

SceneXray::SceneXray(entt::dispatcher& dispatcher) : dispatcher_ { dispatcher } {
    dispatcher_.sink<SceneLoadedEvent>().connect<&SceneXray::on_scene_loaded>(this);
}

void SceneXray::on_scene_loaded(const SceneLoadedEvent& e) {
    scene_ = e.scene;
}

void SceneXray::update() {
    if (!scene_) {
        return;
    }
    ImGui::Begin("Entities");

    for (auto& actor : scene_->actors()) {
        ImGui::Text("[%i] %s", actor->id, actor->archetype->name.c_str());
    }

    ImGui::End();
}
