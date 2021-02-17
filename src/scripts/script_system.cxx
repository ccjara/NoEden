#include "script_system.hxx"

j_script_system::~j_script_system() {
    reset();
}

void j_script_system::on_load() {
    dispatcher_->sink<j_key_down_event>().connect<&j_script_system::on_key_down>(this);
}

void j_script_system::reset() {
    for (auto& [id, script] : scripts_) {
        dispatcher_->trigger<j_script_before_unload_event>(script.get());
    }
    scripts_.clear();
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
    script.run();
    // execute the on_load function, passing the script env proxy
    auto on_load { luabridge::getGlobal(script, "on_load") };
    if (on_load.isFunction()) {
        pcall_into(on_load);
    }
}

void j_script_system::update(uint32_t delta_time) {
}

void j_script_system::reload(j_id_t id) {
    auto it = scripts_.find(id);
    if (it == scripts_.end()) {
        LOG(ERROR) << "Can not reload unknown script " << id;
        return;
    }
    unload(it->first);
    setup(*it->second, true);
}

void j_script_system::unload(j_id_t id) {
    auto it = scripts_.find(id);
    if (it == scripts_.end()) {
        return;
    }
    const std::string name { it->second->name() };
    dispatcher_->trigger<j_script_before_unload_event>(it->second.get());
    scripts_.erase(it);
    LOG(INFO) << "Script " << id << " (" << name << ") has been unloaded";
}

const std::unordered_map<j_id_t, std::unique_ptr<j_script>>& j_script_system::scripts() const {
    return scripts_;
}
