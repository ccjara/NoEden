#include "scripts/api/realm_api.hxx"

#include <realm/realm_type_def.hxx>

#include "realm/realm_manager.hxx"
#include "scripts/scripting.hxx"

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
            .addFunction("current", &RealmApi::current)
        .endClass();

    script.define_enum(
        "RealmType",
        std::make_tuple("MainMenu", RealmType::MainMenu),
        std::make_tuple("World", RealmType::World)
    );

    expose(script, this, "realm");
}

void RealmApi::on_load(luabridge::LuaRef callback) {
    scripting_->add_callback(EventType::RealmLoaded, callback);
}

i32 RealmApi::current() {
    const auto current_realm = realm_manager_->current_realm();

    if (!current_realm) {
        return -1;
    }

    return static_cast<i32> (current_realm->type());
}

EventResult RealmApi::on_realm_loaded(const RealmLoadedEvent& e) {
    for (auto& callback : scripting_->callbacks_by_event_type(EventType::RealmLoaded)) {
        auto state = callback.script->lua_state();

        lua_rawgeti(state, LUA_REGISTRYINDEX, callback.ref);
        lua_pushinteger(state, static_cast<i32> (e.realm->type()));

        if (lua_pcall(state, 1, 0, 0) != LUA_OK) {
            LOG_ERROR("Error calling realm loaded callback: {}", lua_tostring(state, -1));
            lua_pop(state, -1);
        }
    }

    return EventResult::Continue;
}
