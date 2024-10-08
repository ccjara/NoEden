#ifndef NOEDEN_SCRIPT_HXX
#define NOEDEN_SCRIPT_HXX

enum class ScriptStatus {
    Unloaded,
    Loaded,
    Called,
};

enum class ScriptError {
    None,
    RuntimeError,
    StateAllocFailed,
    ScriptCorrupted,
};

/**
 * @brief Wrapper which manages a lua state
 */
class Script {
    friend class ScriptLoader;
public:
    const u64 id;

    /**
     * @brief Instantiates a script from a source string
     */
    explicit Script(std::string&& name, std::string&& source);

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
     * @brief Calls the script by doing a lua pcall
     *
     * Returns false if any error occurred during execution
     */
    bool call();

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

    /**
     * @brief Defines an enum as a globally accessible table in this script
     * 
     * If the enum already exists, the operation will be ignored.
     *
     * Requires the script to be loaded.
     */
    template<typename... kvp>
    void define_enum(const char *enum_name, kvp... key_value_pair) {
        lua_getglobal(state_, enum_name);
        const bool already_exists = !lua_isnil(state_, -1);
        lua_pop(state_, 1);

        if (already_exists) {
            LOG_ERROR("Could not re-define enum {} in script {}", enum_name, name_);
            return;
        }

        lua_newtable(state_);

        (
            ...,
            (
                lua_pushstring(state_, std::get<0>(key_value_pair)), // enum key
                lua_pushinteger(state_, static_cast<i32> (std::get<1>(key_value_pair))), // enum value
                lua_settable(state_, -3)
            )
        );

        lua_setglobal(state_, enum_name);
    }

    ScriptStatus status() const;
    ScriptError error() const;
    const std::string& name() const;
    lua_State* lua_state() const;
    const std::vector<std::string>& globals() const;
    const std::string& source() const;
    void set_source(std::string&& source);

    Script(Script&&) = delete;
    Script& operator=(Script&&) = delete;
    Script(const Script&) = delete;
    Script& operator=(const Script&) = delete;
private:
    static u64 next_id_;

    std::string name_;
    lua_State* state_ = nullptr;
    ScriptStatus status_ = ScriptStatus::Unloaded;
    ScriptError error_ = ScriptError::None;

    std::string source_;

    std::vector<std::string> globals_;

    std::unordered_map<std::string, luabridge::LuaRef> callbacks_;

    void fail(ScriptError err);
};

template<typename t>
void Script::define_global(std::string_view key, t value) {
    if (status_ != ScriptStatus::Loaded) {
        LOG_ERROR("Could not set global {} in script {}: script is not loaded", key, name_);
        return;
    }
    const auto& stored_name { globals_.emplace_back(key) };
    // TODO: this does not cover const char*!
    // concepts / SFINAE....
    if constexpr (!std::is_fundamental<t>::value) { // TODO: "requires script_declarable"
        std::remove_pointer_t<t>::declare(luabridge::getGlobalNamespace(state_));
    }
    luabridge::setGlobal(state_, value, stored_name.c_str());
}

#endif
