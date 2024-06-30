#include "scripts/api/realm_api.hxx"
#include "scripts/scripting.hxx"
#include "realm/realm_manager.hxx"

bool RealmApi::initialize() {
    realm_manager_ = svc_->get<RealmManager>();
    scripting_ = svc_->get<Scripting>();

    realm_loaded_sub_ = events_->on<RealmLoadedEvent>(this, &RealmApi::on_realm_loaded);

    return !!realm_manager_ && !!scripting_;
}

void RealmApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<RealmApi>("Realm")
            .addFunction("on_load", &RealmApi::on_load)
        .endClass();

    expose(script, this, "realm");
}

void RealmApi::on_load(luabridge::LuaRef callback) {
    scripting_->add_callback(EventType::RealmLoaded, callback);
}

EventResult RealmApi::on_realm_loaded(const RealmLoadedEvent& e) {
    const auto& realm_str = realm_type_to_string(e.realm->type());

    for (auto& callback : scripting_->callbacks_by_event_type(EventType::RealmLoaded)) {
        auto state = callback.script->lua_state();

        lua_rawgeti(state, LUA_REGISTRYINDEX, callback.ref);
        lua_pushstring(state, realm_str.c_str());

        if (lua_pcall(state, 1, 0, 0) != LUA_OK) {
            LOG_ERROR("Error calling realm loaded callback: {}", lua_tostring(state, -1));
            lua_pop(state, -1);
        }
    }

    return EventResult::Continue;
}
