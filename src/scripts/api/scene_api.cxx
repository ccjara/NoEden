#include "scene_api.hxx"

void SceneApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<SceneApi>("SceneApi")
            .addFunction("actor_name", &SceneApi::actor_name)
            .addFunction("player_id", &SceneApi::player_id)
            .addFunction("player_name", &SceneApi::player_name)
        .endClass();

    expose(script, this, "scene");
}

const char* SceneApi::actor_name(u64 id) const {
    Actor* actor = Scene::get_actor_by_id(id);
    if (!actor) {
        return nullptr;
    }
    return actor->name.c_str();
}

u64 SceneApi::player_id() const {
    Actor* player = Scene::player();
    if (!player) {
        return null_id;
    }
    return player->id;
}

const char* SceneApi::player_name() const {
    u64 id = player_id();
    if (id == null_id) {
        return nullptr;
    }
    return actor_name(id);
}
