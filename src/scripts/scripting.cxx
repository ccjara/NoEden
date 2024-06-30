#include "scripts/scripting.hxx"

#include "scripts/script_loader.hxx"
#include "scripts/script_registry.hxx"

Scripting::Scripting(ServiceLocator* services, EventManager* events) : svc_(services), events_(events) {
    assert(svc_);
    assert(events_);
    key_down_sub_ = events_->on<KeyDownEvent>(this, &Scripting::on_key_down);
    script_loader_ = std::make_unique<ScriptLoader>();
    script_registry_ = std::make_unique<ScriptRegistry>();
}

Scripting::~Scripting() {
    callbacks_by_script_id_.clear();
    callbacks_by_event_type_.clear();
    script_registry_->reset();
    apis_.clear();
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

    auto* scr_system = script_registry_->script("system");
    if (scr_system) {
        load(*scr_system);
        run(*scr_system);
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
    if (script.status() != ScriptStatus::Called) {
        return;
    }
    {
        // on_unload must go out of scope before script.unload() is called as it frees the lua state
        auto on_unload = luabridge::getGlobal(script, "on_unload");
        if (on_unload.isFunction()) {
            auto unload_result = on_unload();
            if (!unload_result) {
                LOG_ERROR(
                    "Script #{}: {} error in on_unload function: {} ({})",
                    script.id,
                    script.name(),
                    unload_result.errorMessage(),
                    unload_result.errorCode().value()
                );
            }
        }
    }

    clear_callbacks(script.id);

    script.unload();
}

void Scripting::load(Script& script) {
    if (script.status() != ScriptStatus::Unloaded) {
        LOG_ERROR("Could not load script {}: script is already loaded.", script.name());
        return;
    }
    script.load();
    if (script.status() != ScriptStatus::Loaded) {
        return;
    }
    events_->trigger<ScriptLoadedEvent>(&script);
    setup_script_env(script);

    // `Script` does not know of the function based scripting architecture, it just knows
    // that it can be called which in this case is the initial pcall into the loaded script.
    // script.call() sets up the script (by defining functions) and scripting.run()
    // invokes the on_run function in the script.
    // therefore each script is only loaded and thus called once whereas the on_run() function
    // can be called multiple times.
    script.call();
    LOG_INFO("Script #{}: {} has been loaded", script.id, script.name());
}

bool Scripting::run(std::string_view name) {
    auto script = script_registry_->script(name);
    if (!script) {
        LOG_ERROR("Could not run script {}: script not found", name);
        return false;
    }
    return run(*script);
}

bool Scripting::run(Script& script) {
    const auto& name = script.name();

    if (script.status() == ScriptStatus::Unloaded) {
        load(script);
        if (script.status() != ScriptStatus::Called) {
            LOG_ERROR("Could not run script {}: failed to panic-load script", name);
            return false;
        }
    }
    auto on_run = luabridge::getGlobal(script, "on_run");
    if (!on_run.isFunction()) {
        LOG_ERROR("Script {} does not have an on_run function", name);
        return false;
    }
    auto run_result = on_run();
    if (!run_result) {
        LOG_ERROR(
            "Script {} error in on_run function: {} ({})",
            name,
            run_result.errorMessage(),
            run_result.errorCode().value()
        );
        return false;
    }
    return true;
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

Script* Scripting::get_by_lua_ref(luabridge::LuaRef& ref) {
    Id id = null_id;
    auto state = ref.state();

    lua_getglobal(state, "self");

    if (lua_istable(state, -1)) {
        lua_getfield(state, -1, "id");
        if (lua_isinteger(state, -1)) {
            id = lua_tointeger(state, -1);
        }
        lua_pop(state, 1);
    }

    lua_pop(state, 1);

    if (id == null_id) {
        return nullptr;
    }

    return get_by_id(id);
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
    lua_setglobal(script, "self");

    events_->trigger<ScriptEnvSetupEvent>(&script);
}

void Scripting::add_callback(EventType event_type, luabridge::LuaRef& callback) {
    if (!callback.isFunction()) {
        LOG_ERROR("Could not add callback for event {}: callback must be a function", static_cast<i32> (event_type));
        return;
    }
    Script* script = get_by_lua_ref(callback);
    if (script == nullptr) {
        LOG_ERROR("Could not add callback for event {}: script not found", static_cast<i32> (event_type));
        return;
    }

    callback.push();
    const i32 callback_ref = luaL_ref(script->lua_state(), LUA_REGISTRYINDEX);

    callbacks_by_event_type_[event_type].emplace_back(script, callback_ref);
    callbacks_by_script_id_[script->id].emplace_back(script, callback_ref);
}


void Scripting::clear_callbacks(Id script_id) {
    if (!callbacks_by_script_id_.contains(script_id)) {
        return;
    }
    callbacks_by_script_id_[script_id].clear();
}

const std::vector<LuaCallback>& Scripting::callbacks_by_event_type(EventType event_type) const {
    if (!callbacks_by_event_type_.contains(event_type)) {
        return empty_callbacks_;
    }
    return callbacks_by_event_type_.at(event_type);
}

const std::vector<LuaCallback>& Scripting::callbacks_by_script_id(Id script_id) const {
    if (!callbacks_by_script_id_.contains(script_id)) {
        return empty_callbacks_;
    }
    return callbacks_by_script_id_.at(script_id);
}
