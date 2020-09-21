#ifndef JARALYN_SCRIPT_HXX
#define JARALYN_SCRIPT_HXX

enum class j_script_status {
    indeterminate,
    error,
    loaded,
    called,
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
class j_script {
private:
    std::string id_;
    lua_State* state_ { nullptr };
    j_script_status status_ { j_script_status::indeterminate };
    j_script_error error_ { j_script_error::none };

    std::string path_;
    std::string source_;

    void fail(j_script_error err);
public:
    /**
     * @brief Loads a script from the given path
     *
     * If the file does not exist, the status flag will be switched
     * to `error` and the script state will be empty (but remain allocated).
     */
    j_script(const std::string& id, const fs::path& path);

    /**
     * @brief Loads a script from a source string
     */
    j_script(const std::string& id, const std::string& source);

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
     *
     * A script can be loaded multiple times. The previous script state is
     * discarded and reallocated on each load.
     */
    void load();

    j_script_status status() const;
    bool called() const;
    bool callable() const;
    bool loaded() const;
    const std::string& id() const;
    lua_State* lua_state() const;

    // move the managed script state between j_script instances
    j_script(j_script&&);
    j_script& operator=(j_script&&);

    // not sure if script copies are needed, disable for now
    j_script(const j_script&) = delete;
    j_script& operator=(const j_script&) = delete;
};

#endif
