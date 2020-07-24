#include "script_env.hxx"

void j_script_env::on_inventory_item_added(const j_inventory_item_added_event& e) {
    const auto it { listeners_.find(j_game_event_type::inventory_item_added) };
    if (it == listeners_.end()) {
        return;
    }
    for (auto& listener : it->second) {
        pcall_into(listener, e.item()->label.c_str());
    }
}

bool j_script_env::on_register_callback(const char* event_type, luabridge::LuaRef ref) {
    LOG(INFO) << "Lua callback requested on game event " << event_type;
    const auto entry { event_type_by_string.find(event_type) };

    if (entry == event_type_by_string.end()) {
        LOG(ERROR) << "Cannot register game event callback '" << event_type << "': unknown event type";
        return false;
    }
    listeners_[entry->second].push_back(std::move(ref));
    return true;
}

void j_script_env::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<j_inventory_item_added_event>().connect<&j_script_env::on_inventory_item_added>(this);
}

void j_script_env::setup(j_script& script) {
    if (script.status() == j_script_status::error) {
        return;
    }

    luaL_openlibs(script);

    luabridge::getGlobalNamespace(script)
        .beginClass<j_script_env>("env")
            .addFunction("on", &j_script_env::on_register_callback)
        .endClass();

    script.run();
  
    auto on_load = luabridge::getGlobal(script, "on_load");
    if (on_load.isFunction()) {
        pcall_into(on_load, this);
    }
}
