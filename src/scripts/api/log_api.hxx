#ifndef NOEDEN_LOG_API_HXX
#define NOEDEN_LOG_API_HXX

#include "lua_api.hxx"

class LogApi : public LuaApi {
public:
    void on_register(Script& script) override;

    /**
     * @brief Called from lua to log a message on error level
     */
    void log_error(const char* message) const;

    /**
     * @brief Called from lua to log a message on info level
     */
    void log_info(const char* message) const;

    /**
     * @brief Called from lua to log a message on warn level
     */
    void log_warn(const char* message) const;

    /**
     * @brief Called from lua to log a message on debug level
     */
    void log_debug(const char* message) const;
};

#endif
