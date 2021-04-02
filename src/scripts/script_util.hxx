#ifndef JARALYN_SCRIPT_UTIL_HXX
#define JARALYN_SCRIPT_UTIL_HXX

/**
 * @brief Does a protected call into the given lua ref
 *
 * Checks whether the call was successful. In case of an error, the error
 * is logged, *no* exception is thrown.
 */
template<typename... varg_t>
void pcall_into(luabridge::LuaRef& ref, varg_t&&... args) {
    const auto result { ref(std::forward<varg_t>(args)...) };
    if (result == std::nullopt) {
        const auto state { ref.state() };

        std::string err { "Caught error during script execution" };

        if (lua_gettop(state) > 0) {
            const char* e = lua_tostring(state, -1);
            if (e) {
                err.append(": ");
                err.append(e);
            }
        }
        LOG(ERROR) << err;
    }
}

#endif
