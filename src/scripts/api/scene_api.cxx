#include "scene_api.hxx"

void SceneApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<SceneApi>("SceneApi")
            .addFunction("entity_name", &SceneApi::entity_name)
            .addFunction("player_id", &SceneApi::player_id)
            .addFunction("player_name", &SceneApi::player_name)
        .endClass();

    expose(script, this, "scene");
}

const char* SceneApi::entity_name(u64 id) const {
    Entity* entity = Scene::get_entity_by_id(id);
    if (!entity) {
        return nullptr;
    }
    return entity->name.c_str();
}

u64 SceneApi::player_id() const {
    Entity* player = Scene::player();
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
    return entity_name(id);
}
