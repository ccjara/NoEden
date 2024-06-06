#include "scripts/scripting.hxx"

Scripting::Scripting(EventManager* events) : events_(events) {
    assert(events_);
    events_->on<KeyDownEvent>(this, &Scripting::on_key_down);
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

    {
        auto result = script_loader_->load_from_directory(default_script_path);
        if (result.error != LoadFromDirectoryError::None) {
            return;
        }
        script_registry_->reset();
        script_registry_->add(std::move(result.scripts));
    }
    for (auto& kvp : script_registry_->scripts()) {
        load(*kvp.second);
    }
}

bool Scripting::on_key_down(KeyDownEvent& e) {
    if (e.key != Key::F5) {
        return false;
    }
    reload();
    return true;
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
    setup_script_env(script);

    // allow other parts of the system to contribute to the scripting env
    events_->trigger<ScriptLoadedEvent>(&script);
    Log::info("Script #{}: {} has been loaded", script.id, script.name());
    script.run();
    // execute the on_load function, passing the script env proxy
    auto on_load { luabridge::getGlobal(script, "on_load") };
    if (on_load.isFunction()) {
        pcall_into(on_load);
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

bool Scripting::register_lua_callback(lua_event_type event_type, luabridge::LuaRef ref) {
    const auto script_id {
        luabridge::getGlobal(ref.state(), "script_id").cast<u64>()
    };
    if (!ref.isFunction()) {
        Log::error("Could not register lua callback for {} on script #{}: Lua callback must be a function", event_type, script_id);
        return false;
    }
    listeners_[event_type].emplace_back(ScriptRef{ script_id, ref });
    Log::info("Lua callback for {} registered in script #{}", event_type, script_id);
    return true;
}

void Scripting::setup_script_env(Script& script) {
    // when calling back from lua to engine code it is often useful to identify
    // the calling script. not sure how else to do it.
    luabridge::setGlobal(script, script.id, "script_id"); // TODO: ensure immutability
    luabridge::setGlobal(script, script.name().c_str(), "script_name"); // TODO: ensure immutability

    luabridge::getGlobalNamespace(script)
        .beginClass<Scripting>("Script")
            .addFunction("on", &Scripting::register_lua_callback)
        .endClass();
    // expose all registered apis
    for (auto& api : apis_) {
        api->on_register(&script);
    }
}
