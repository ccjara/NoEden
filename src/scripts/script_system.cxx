#include "script_system.hxx"

j_script_system::j_script_system(entt::dispatcher* const dispatcher) :
    dispatcher_(dispatcher) {
    assert(dispatcher_);
}

j_script_system::~j_script_system() noexcept {
    // need to clear all refs ahead of the scripts, otherwise the refs are
    // TODO: bind the refs to the j_script instances somehow so the script 
    //       can get rid of them?
    listeners_.clear();
    scripts_.clear();
}

void j_script_system::attach(entt::dispatcher& dispatcher) noexcept {
    dispatcher.sink<j_inventory_item_added_event>().connect<&j_script_system::on_inventory_item_added>(this);
    dispatcher.sink<j_scene_render_event>().connect<&j_script_system::on_scene_render>(this);
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

void j_script_system::on_scene_render(const j_scene_render_event& e) {
    const auto it { scene_render_listeners_.find(e.scene_type()) };
    if (it == scene_render_listeners_.end()) {
        return;
    }
    j_display_proxy display(e.display());
    for (auto& bound_ref : it->second) {
        pcall_into(bound_ref.ref, display);
    }
}

bool j_script_system::on_register_callback(const char* event_type, luabridge::LuaRef ref) {
    LOG(INFO) << "Lua callback requested on game event " << event_type;
    const auto entry { event_type_by_string.find(event_type) };
    if (entry == event_type_by_string.end()) {
        LOG(ERROR) << "Cannot register game event callback '" << event_type << "': unknown event type";
        return false;
    }
    auto state = ref.state();
    auto script_id = luabridge::getGlobal(state, "script_id").cast<const char*>();

    listeners_[entry->second].emplace_back(script_id, std::move(ref));
    return true;
}

bool j_script_system::on_register_renderer(const char *scene_str, luabridge::LuaRef ref) {
    LOG(INFO) << "Renderer registration requested for scene " << scene_str;
    const auto iter { scene_type_by_string.find(scene_str) };
    if (iter == scene_type_by_string.end()) {
        LOG(ERROR) << "Cannot register renderer for scene " << scene_str << ": unknown scene type";
        return false;
    }
    auto state = ref.state();
    auto script_id = luabridge::getGlobal(state, "script_id").cast<const char*>();

    scene_render_listeners_[iter->second].emplace_back(script_id, std::move(ref));
    return true;
}

void j_script_system::setup(j_script& script) {
    script.load();

    luaL_openlibs(script);

    luabridge::setGlobal(script, script.id().c_str(), "script_id");

    luabridge::getGlobalNamespace(script)
        .beginClass<j_script_system>("env")
        .addFunction("on", &j_script_system::on_register_callback)
        .addFunction("register_renderer", &j_script_system::on_register_renderer)
        .endClass()

        .beginClass<j_display_proxy>("display")
        .addFunction("text", &j_display_proxy::text)
        .endClass();

    script.run();

    auto on_load = luabridge::getGlobal(script, "on_load");
    if (on_load.isFunction()) {
        pcall_into(on_load, this);
    }
}
