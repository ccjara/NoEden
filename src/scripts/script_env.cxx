#include "script_env.hxx"

void j_script_env::on_inventory_item_added(const j_inventory_item_added_event& e) {
    const auto it { listeners_.find("INVENTORY_ITEM_ADDED") };
    if (it == listeners_.end()) {
        return;
    }
    for (auto& l : it->second) {
        pcall_into(l, e.item()->label.c_str());
    }
}

void j_script_env::on_register_callback(const char* event_type, luabridge::LuaRef ref) {
    LOG(INFO) << "Lua callback requested on world event " << event_type;

    listeners_[event_type].push_back(std::move(ref));
}

void j_script_env::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<j_inventory_item_added_event>().connect<&j_script_env::on_inventory_item_added>(this);
}

void j_script_env::setup(j_script& script) {
    if (script.status() == j_script_status::error) {
        return;
    }

    auto state { script.lua_state() };

    luaL_openlibs(state);

    luabridge::getGlobalNamespace(state)
        .beginClass<j_script_env>("env")
            .addFunction("on", &j_script_env::on_register_callback)
        .endClass();

    script.run();
  
    auto on_load = luabridge::getGlobal(state, "on_load");
    if (on_load.isFunction()) {
        pcall_into(on_load, this);
    }
}
