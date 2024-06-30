#ifndef NOEDEN_SCRIPT_REGISTRY_HXX
#define NOEDEN_SCRIPT_REGISTRY_HXX

class Script;

class ScriptRegistry {
public:
    void reset();
    void add(std::vector<std::unique_ptr<Script>>&& scripts);

    const std::unordered_map<u64, std::unique_ptr<Script>>& scripts() const;
    std::unordered_map<u64, std::unique_ptr<Script>>& scripts();

    Script* script(u64 id);
    Script* script(std::string_view name);

    ~ScriptRegistry();
    ScriptRegistry() = default;
    ScriptRegistry(const ScriptRegistry&) = delete;
    ScriptRegistry& operator=(const ScriptRegistry&) = delete;
    ScriptRegistry(ScriptRegistry&&) = delete;
    ScriptRegistry& operator=(ScriptRegistry&&) = delete;
private:
    std::unordered_map<u64, std::unique_ptr<Script>> scripts_ = {};
    std::unordered_map<std::string, u64> script_names_ = {};
};

#endif
