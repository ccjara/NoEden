#ifndef JARALYN_SCRIPT_HXX
#define JARALYN_SCRIPT_HXX

enum class j_script_status {
    indeterminate,
    loaded,
    error,
};

/**
 * @brief Wrapper which manages a lua state
 */
class j_script {
private:
    std::string id_;
    lua_State* state_ { nullptr };
    j_script_status status_ { j_script_status::indeterminate };
    bool has_run_ { false };

    std::string path_;
    std::string source_;
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
    ~j_script() noexcept;

    /**
     * @brief Implicit conversion from a j_script instance to a lua state
     *
     * This allows passing a j_script to lua C functions.
     */
    operator lua_State* () const noexcept;

    /**
     * @brief Runs the script by doing a lua pcall
     *
     * Returns false if any error occurred during execution
     */
    bool run();

    /**
     * @brief Loads the script from its configured data source
     */
    void load();

    /**
     * @brief Reloads the script from its data source
     */
    void reload();

    j_script_status status() const noexcept;
    bool loaded() const noexcept;
    bool has_run() const noexcept;
    const std::string& id() const noexcept;
    lua_State* lua_state() const noexcept;

    // move the managed script state between j_script instances
    j_script(j_script&&);
    j_script& operator=(j_script&&);

    // not sure if script copies are needed, disable for now
    j_script(const j_script&) = delete;
    j_script& operator=(const j_script&) = delete;
};

#endif
