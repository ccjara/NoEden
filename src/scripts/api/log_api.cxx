#include "log_api.hxx"

void LogApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<LogApi>("LogApi")
            .addFunction("debug", &LogApi::log_debug)
            .addFunction("info", &LogApi::log_info)
            .addFunction("warn", &LogApi::log_warn)
            .addFunction("error", &LogApi::log_error)
        .endClass();

    expose(script, this, "log");
}

void LogApi::log_debug(const char* message) const {
    LOG_DEBUG("{}", message);
}

void LogApi::log_info(const char* message) const {
    LOG_INFO("{}", message);
}

void LogApi::log_warn(const char* message) const {
    LOG_WARN("{}", message);
}

void LogApi::log_error(const char* message) const {
    LOG_ERROR("{}", message);
}
