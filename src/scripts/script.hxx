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
    lua_State* state_ { nullptr };
    j_script_status status_ { j_script_status::indeterminate };
public:
    /**
     * @brief Allocates a new lua state
     */
    j_script();

    /**
     * @brief Frees the currently managed lua state if allocated
     */
    ~j_script() noexcept;

    /**
     * @brief Converts a j_script instance to a lua state
     */
    operator lua_State* () const noexcept;

    /**
     * @brief Loads the script from the given path
     *
     * If the file does not exist, the status flag will be switched
     * to `error` and the script state will be empty (but remain allocated).
     */
    static j_script from_file(const char* path);

    j_script_status status() const noexcept;
    bool loaded() const noexcept;

    // move the managed script state between j_script instances
    j_script(j_script&&);
    j_script& operator=(j_script&&);

    // not sure if script copies are needed, disable for now
    j_script(const j_script&) = delete;
    j_script& operator=(const j_script&) = delete;
};

#endif
