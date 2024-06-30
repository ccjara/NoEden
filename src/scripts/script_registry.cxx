#include "scripts/script_registry.hxx"
#include "scripts/script.hxx"

ScriptRegistry::~ScriptRegistry() {
    reset();
}

void ScriptRegistry::reset() {
    scripts_.clear();
    script_names_.clear();
}

const std::unordered_map<u64, std::unique_ptr<Script>>& ScriptRegistry::scripts() const {
    return scripts_;
}

std::unordered_map<u64, std::unique_ptr<Script>>& ScriptRegistry::scripts() {
    return scripts_;
}

Script* ScriptRegistry::script(u64 id) {
    const auto it = scripts_.find(id);
    return it == scripts_.end() ? nullptr : it->second.get();
}

Script* ScriptRegistry::script(std::string_view name) {
    const auto it = script_names_.find(std::string(name));
    return it == script_names_.end() ? nullptr : script(it->second);
}

void ScriptRegistry::add(std::vector<std::unique_ptr<Script>>&& scripts) {
    for (auto& script_ptr: scripts) {
        auto id = script_ptr->id;
        script_names_.emplace(script_ptr->name(), id);
        scripts_.emplace(id, std::move(script_ptr));
    }
}
