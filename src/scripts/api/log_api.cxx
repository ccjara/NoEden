#include "log_api.hxx"

void LogApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<LogApi>("LogApi")
            .addFunction("debug", &LogApi::log_debug)
            .addFunction("info", &LogApi::log_info)
            .addFunction("warn", &LogApi::log_warn)
            .addFunction("error", &LogApi::log_error)
        .endClass();

    expose(script, this, "log");
}

void LogApi::log_debug(const char *message, lua_State* state) const {
    Log::debug(create_tagged_log_string(message, state));
}

void LogApi::log_info(const char *message, lua_State* state) const {
    Log::info(create_tagged_log_string(message, state));
}

void LogApi::log_warn(const char *message, lua_State* state) const {
    Log::warn(create_tagged_log_string(message, state));
}

void LogApi::log_error(const char *message, lua_State* state) const {
    Log::error(create_tagged_log_string(message, state));
}

std::string LogApi::create_tagged_log_string(const char *message, lua_State* state) const {
    const auto ref = luabridge::getGlobal(state, "script_name");
    std::string script_name;
    if (ref.isString()) {
        script_name = ref.cast<const char*>();
    } else {
        script_name = "Unknown script";
    }
    return fmt::format("[LUA: {}] {}", script_name, message);
}
