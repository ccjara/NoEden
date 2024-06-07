#include "scripts/scripting.hxx"

Scripting::Scripting(Events* events) : events_(events) {
    assert(events_);
    events_->engine->on<KeyDownEvent>(this, &Scripting::on_key_down);
    script_loader_ = std::make_unique<ScriptLoader>();
    script_registry_ = std::make_unique<ScriptRegistry>();
}

Scripting::~Scripting() {
    script_registry_->reset();
    apis_.clear();
}

void Scripting::reset() {
    events_->engine->trigger<ScriptResetEvent>();
    script_registry_->reset();
}

void Scripting::reload() {
    events_->engine->trigger<ScriptResetEvent>();

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

    for (auto& api : apis_) {
        api->on_register(script);
    }

    events_->engine->trigger<ScriptLoadedEvent>(&script);
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
