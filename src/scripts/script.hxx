#ifndef JARALYN_SCRIPT_HXX
#define JARALYN_SCRIPT_HXX

enum class j_script_status {
    unloaded,
    loaded,
    executed,
};

enum class j_script_error {
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
class j_script : public j_identity<j_script> {
    friend class j_script_system;
private:
    std::string name_;
    lua_State* state_ { nullptr };
    j_script_status status_ { j_script_status::unloaded };
    j_script_error error_ { j_script_error::none };

    std::string path_;
    std::string source_;

    std::vector<std::string> globals_;

    std::unordered_map<std::string, luabridge::LuaRef> callbacks_;

    void fail(j_script_error err);
public:
    /**
     * @brief Instantiates a script from a source string
     */
    j_script(const std::string& name);

    /**
     * @brief Frees the currently managed lua state if allocated
     */
    ~j_script();

    /**
     * @brief Implicit conversion from a j_script instance to a lua state
     *
     * This allows passing a j_script to lua C functions.
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

    j_script_status status() const;
    const std::string& name() const;
    lua_State* lua_state() const;
    const std::vector<std::string>& globals() const;
    const std::string& source() const;
    void set_source(std::string&& source);

    // move the managed script state between j_script instances
    j_script(j_script&&);
    j_script& operator=(j_script&&);

    // not sure if script copies are needed, disable for now
    j_script(const j_script&) = delete;
    j_script& operator=(const j_script&) = delete;
};

template<typename t>
void j_script::define_global(std::string_view name, t value) {
    if (status_ != j_script_status::loaded) {
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
