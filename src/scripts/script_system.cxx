#include "script_system.hxx"

j_script_system::~j_script_system() {
    reset();
}

void j_script_system::on_load() {
    dispatcher_->sink<j_item_stored_event>().connect<&j_script_system::on_item_stored>(this);
    dispatcher_->sink<j_key_down_event>().connect<&j_script_system::on_key_down>(this);
}

void j_script_system::reset() {
    for (auto& entry : scripts_) {
        dispatcher_->trigger<j_script_before_unload_event>(&entry.second);
    }
    listeners_.clear();
    scripts_.clear();
}

void j_script_system::on_item_stored(const j_item_stored_event& e) {
    const auto it { listeners_.find(j_public_event_type::inventory_item_added) };
    if (it == listeners_.end()) {
        return;
    }
    for (auto& bound_ref : it->second) {
        pcall_into(bound_ref.ref, e.item);
    }
}

bool j_script_system::on_register_callback(const char* event_type, luabridge::LuaRef ref) {
    LOG(INFO) << "Lua callback requested on event " << event_type;
    const auto entry { event_type_by_string.find(event_type) };
    if (entry == event_type_by_string.end()) {
        LOG(ERROR) << "Cannot register event callback '" << event_type << "': unknown event type";
        return false;
    }
    auto state = ref.state();
    auto script_id = luabridge::getGlobal(state, "script_id").cast<const char*>();

    listeners_[entry->second].emplace_back(script_id, std::move(ref));
    return true;
}

void j_script_system::on_key_down(const j_key_down_event& e) {
    if (e.key == SDLK_F5) {
        preload(default_script_path);
    }
}

void j_script_system::setup(j_script& script, bool reloaded) {
    script.load();
    if (!script.loaded()) {
        return;
    }
    // allow other parts of the system contribute to the scripting env
    dispatcher_->trigger<j_script_loaded_event>(&script, reloaded);
    // declare the env arg that will be passed into the on_load function
    luabridge::getGlobalNamespace(script)
        .beginClass<j_script_system>("events")
        .addFunction("on", &j_script_system::on_register_callback)
        .endClass();

    luabridge::setGlobal(script, script.id().c_str(), "script_id");
    
    script.run();
    // execute the on_load function, passing the script env proxy
    auto on_load { luabridge::getGlobal(script, "on_load") };
    if (on_load.isFunction()) {
        pcall_into(on_load, this);
    }
}

void j_script_system::update(uint32_t delta_time) {
}
