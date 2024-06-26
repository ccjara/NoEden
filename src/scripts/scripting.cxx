#include "scripts/scripting.hxx"

Scripting::Scripting(EventManager* events) : events_(events) {
    assert(events_);
    key_down_sub_ = events_->on<KeyDownEvent>(this, &Scripting::on_key_down);
    script_loader_ = std::make_unique<ScriptLoader>();
    script_registry_ = std::make_unique<ScriptRegistry>();
}

Scripting::~Scripting() {
    script_registry_->reset();
    apis_.clear();
}

void Scripting::reset() {
    events_->trigger<ScriptResetEvent>();
    script_registry_->reset();
}

void Scripting::reload() {
    events_->trigger<ScriptResetEvent>();
    for (auto& script: script_registry_->scripts()) {
        unload(*script.second);
    }

    {
        auto result = script_loader_->load_from_directory(default_script_path);
        if (result.error != LoadFromDirectoryError::None) {
            return;
        }
        script_registry_->reset();
        script_registry_->add(std::move(result.scripts));
    }
    for (auto& kvp: script_registry_->scripts()) {
        load(*kvp.second);
    }
}

EventResult Scripting::on_key_down(KeyDownEvent& e) {
    if (e.key != Key::F5) {
        return EventResult::Continue;
    }
    reload();
    return EventResult::Halt;
}

void Scripting::unload(Script& script) {
    if (script.status() != ScriptStatus::Executed) {
        return;
    }
    {
        // on_unload must go out of scope before script.unload() is called as it frees the lua state
        auto on_unload = luabridge::getGlobal(script, "on_unload");
        if (on_unload.isFunction()) {
            auto unload_result = on_unload();
            if (!unload_result) {
                Log::error(
                    "Script #{}: {} error in on_unload function: {} ({})",
                    script.id,
                    script.name(),
                    unload_result.errorMessage(),
                    unload_result.errorCode().value()
                );
            }
        }
    }
    script.unload();
}

void Scripting::load(Script& script) {
    if (script.status() != ScriptStatus::Unloaded) {
        Log::error("Could not load script {}: script is already loaded.", script.name());
        return;
    }
    script.load();
    if (script.status() != ScriptStatus::Loaded) {
        return;
    }
    events_->trigger<ScriptLoadedEvent>(&script);
    setup_script_env(script);

    events_->trigger<ScriptEnvSetupEvent>(&script);
    Log::info("Script #{}: {} has been loaded", script.id, script.name());
    if (!script.run()) {
        return;
    }

    auto on_load = luabridge::getGlobal(script, "on_load");
    if (on_load.isFunction()) {
        auto load_result = on_load();
        if (!load_result) {
            Log::error(
                "Script #{}: {} error in on_load function: {} ({})",
                script.id,
                script.name(),
                load_result.errorMessage(),
                load_result.errorCode().value()
            );
        }
    }
}

const std::unordered_map<u64, std::unique_ptr<Script>>& Scripting::scripts() const {
    return script_registry_->scripts();
}

std::unordered_map<u64, std::unique_ptr<Script>>& Scripting::scripts() {
    return script_registry_->scripts();
}

Script* Scripting::get_by_id(u64 id) {
    return script_registry_->script(id);
}

void Scripting::setup_script_env(Script& script) {
    assert(script.status() == ScriptStatus::Loaded);

    for (auto& api: apis_) {
        api->on_register(script);
    }

    lua_newtable(script);

    lua_pushstring(script, script.name().c_str());
    lua_setfield(script, -2, "name");

    lua_pushinteger(script, script.id);
    lua_setfield(script, -2, "id");

    // create and set script metatable
    lua_newtable(script);
    lua_pushstring(script, "__newindex");
    lua_pushcfunction(script, [](lua_State* L) -> int {
        const char* key = lua_tostring(L, 2);
        luaL_error(L, "attempt to modify read-only variable '%s'", key);
        return 0;
    });
    lua_settable(script, -3);
    lua_setmetatable(script, -2);
    lua_setglobal(script, "script");
}
