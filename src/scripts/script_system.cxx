#include "script_system.hxx"

void j_script_system::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<j_inventory_item_added_event>().connect<&j_script_system::on_inventory_item_added>(this);
}

void j_script_system::on_inventory_item_added(const j_inventory_item_added_event& e) {
    const auto it { listeners_.find(j_game_event_type::inventory_item_added) };
    if (it == listeners_.end()) {
        return;
    }
    for (auto& bound_ref : it->second) {
        pcall_into(bound_ref.ref, e.item()->label.c_str());
    }
}

bool j_script_system::on_register_callback(const char* event_type, luabridge::LuaRef ref) {
    LOG(INFO) << "Lua callback requested on game event " << event_type;
    const auto entry { event_type_by_string.find(event_type) };

    auto state = ref.state();
    auto script_id = luabridge::getGlobal(state, "script_id").cast<const char*>();

    if (entry == event_type_by_string.end()) {
        LOG(ERROR) << "Cannot register game event callback '" << event_type << "': unknown event type";
        return false;
    }
    listeners_[entry->second].push_back({ script_id, std::move(ref) });
    return true;
}

void j_script_system::setup(j_script& script) {
    script.load();

    luaL_openlibs(script);

    luabridge::setGlobal(script, script.id().c_str(), "script_id");

    luabridge::getGlobalNamespace(script)
        .beginClass<j_script_system>("env")
        .addFunction("on", &j_script_system::on_register_callback)
        .endClass();

    script.run();

    auto on_load = luabridge::getGlobal(script, "on_load");
    if (on_load.isFunction()) {
        pcall_into(on_load, this);
    }
}
