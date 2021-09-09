#include "scripts.hxx"

Scripts::Scripts(entt::dispatcher& dispatcher) : dispatcher_ { dispatcher } {
    dispatcher_.sink<KeyDownEvent>().connect<&Scripts::on_key_down>(this);
}

Scripts::~Scripts() {
    reset();
}

void Scripts::reset() {
    dispatcher_.trigger<ScriptResetEvent>();
    listeners_.clear();
    scripts_.clear();
}

void Scripts::on_key_down(const KeyDownEvent& e) {
    if (e.key == Key::F5) {
        load_from_path(default_script_path);
    }
}

void Scripts::load(Script& script) {
    if (script.status() != ScriptStatus::unloaded) {
        LOG(ERROR) << "Cannot load script "
                   << script.name() << ": script is already loaded. "
                   << "Call reload instead.";
        return;
    }
    script.load();
    if (script.status() != ScriptStatus::loaded) {
        return;
    }
    setup_script_env(script);

    // allow other parts of the system to contribute to the scripting env
    dispatcher_.trigger<ScriptLoadedEvent>(&script);
    LOG(INFO) << "Script " << script.name() << " (" << script.id << ") has been loaded";
    script.run();
    // execute the on_load function, passing the script env proxy
    auto on_load { luabridge::getGlobal(script, "on_load") };
    if (on_load.isFunction()) {
        pcall_into(on_load);
    }
}

const std::unordered_map<u64, std::unique_ptr<Script>>& Scripts::scripts() const {
    return scripts_;
}

Script* Scripts::get_by_id(u64 id) const {
    const auto it { scripts_.find(id) };
    return it == scripts_.end() ? nullptr : it->second.get();
}

bool Scripts::register_lua_callback(lua_event_type event_type, luabridge::LuaRef ref) {
    const auto script_id {
        luabridge::getGlobal(ref.state(), "script_id").cast<u64>()
    };
    if (!ref.isFunction()) {
        LOG(ERROR) << "Lua callback must be a function";
        return false;
    }
    listeners_[event_type].emplace_back(ScriptRef{ script_id, ref });
    LOG(INFO) << "Lua callback registered on event type " << event_type;
    return true;
}

void Scripts::setup_script_env(Script& script) {
    // when calling back from lua to engine code it is often useful to identify
    // the calling script. not sure how else to do it.
    luabridge::setGlobal(script, script.id, "script_id"); // TODO: ensure immutability

    luabridge::getGlobalNamespace(script)
        .beginClass<Scripts>("Script")
            .addFunction("on", &Scripts::register_lua_callback)
        .endClass();
    // expose some lua facing methods the system provides
    luabridge::setGlobal(script, this, "script");
}