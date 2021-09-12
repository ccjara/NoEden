#ifndef JARALYN_SCRIPT_UTIL_HXX
#define JARALYN_SCRIPT_UTIL_HXX

/**
 * @brief Invokes a protected call on the given lua ref
 *
 * Checks whether the call was successful. In case of an error, the error
 * is logged, *no* exception is thrown.
 */
template<typename... varg_t>
void pcall_into(luabridge::LuaRef& ref, varg_t&&... args) {
    const luabridge::LuaResult result { ref(std::forward<varg_t>(args)...) };

    if (!result) {
        LOG(ERROR) << "Caught error during script execution: "
                   << result.errorMessage() << " (" << result.errorCode() << ")";
    }
}

#endif
