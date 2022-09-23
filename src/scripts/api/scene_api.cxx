#include "scene_api.hxx"

void SceneApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<SceneApi>("SceneApi")
            .addFunction("actor_name", &SceneApi::actor_name)
        .endClass();

    expose(script, this, "scene");
}

const char *SceneApi::actor_name(u64 id) const {
    Actor* actor = Scene::get_actor_by_id(id);
    if (!actor) {
        return "Unknown";
    }
    return actor->archetype->name.c_str();
}