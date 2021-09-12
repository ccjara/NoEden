#ifndef JARALYN_SCRIPT_HXX
#define JARALYN_SCRIPT_HXX

enum class ScriptStatus {
    unloaded,
    loaded,
    executed,
};

enum class ScriptError {
    none,
    runtime_error,
    state_alloc_failed,
    script_path_not_found,
    bad_script_input,
    script_corrupted,
};

/**
 * @brief Wrapper which manages a lua state
 */
class Script {
    friend class Scripting;
private:
    static u64 next_id_;

    std::string name_;
    lua_State* state_ { nullptr };
    ScriptStatus status_ { ScriptStatus::unloaded };
    ScriptError error_ { ScriptError::none };

    std::string path_;
    std::string source_;

    std::vector<std::string> globals_;

    std::unordered_map<std::string, luabridge::LuaRef> callbacks_;

    void fail(ScriptError err);
public:
    const u64 id;

    /**
     * @brief Instantiates a script from a source string
     */
    Script(const std::string& name);

    /**
     * @brief Frees the currently managed lua state if allocated
     */
    ~Script();

    /**
     * @brief Implicit conversion from a script instance to a lua state
     *
     * This allows passing a Script to lua C functions.
     */
    operator lua_State* () const;

    /**
     * @brief Runs the script by doing a lua pcall
     *
     * Returns false if any error occurred during execution
     */
    bool run();

    /**
     * @brief Loads the script from its configured data source
     *
     * Sets the script status to `loaded` if loaded successfully or
     * `error` on failure. This also sets the `error_` member accordingly.
     */
    void load();

    /**
     * @brief Unloads the script, freeing its resources
     */
    void unload();

    template<typename t>
    void define_global(std::string_view name, t value);

    template<typename... types>
    inline void declare() {
        (types::declare(luabridge::getGlobalNamespace(state_)), ...);
    }

    ScriptStatus status() const;
    const std::string& name() const;
    lua_State* lua_state() const;
    const std::vector<std::string>& globals() const;
    const std::string& source() const;
    void set_source(std::string&& source);

    Script(Script&&) = delete;
    Script& operator=(Script&&) = delete;
    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;
};

template<typename t>
void Script::define_global(std::string_view name, t value) {
    if (status_ != ScriptStatus::loaded) {
        LOG(ERROR) << "Cannot set global '" << name << "' in script '" << name_ << "': "
                   << "script is not loaded";
        return;
    }
    const auto& stored_name { globals_.emplace_back(name) };
    // TODO: this does not cover const char*!
    // concepts / SFINAE....
    if constexpr (!std::is_fundamental<t>::value) { // TODO: "requires script_declarable"
        std::remove_pointer_t<t>::declare(luabridge::getGlobalNamespace(state_));
    }
    luabridge::setGlobal(state_, value, stored_name.c_str());
}

#endif
