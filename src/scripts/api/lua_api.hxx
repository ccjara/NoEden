#ifndef JARALYN_LUA_API
#define JARALYN_LUA_API

#include "../../scripts/script.hxx"

/**
 * @brief Interface for lua api implementations.
 */
class LuaApi {
    friend class ApiRegistrar;
public:
    virtual ~LuaApi() = default;

    /**
     * @brief Called for each loaded script.
     * 
     * Namespaces and globals for each script should be setup here.
     */
    virtual void on_register(Script* script) = 0;

    /**
     * @brief Exposes the given api to lua under the given name.
     */
    template<typename Api>
    void expose(Script* script, Api* api, const char* name) {
        std::error_code ec;
        luabridge::push<Api*>(*script, api, ec);
        if (ec) {
            Log::error("Could not expose api {}: {}", name, ec.message());
            return;
        }
        lua_setglobal(*script, name);
    }
protected:
    /**
     * @brief Injected by the Scripting class upon creation of an API instance.
     */
    entt::dispatcher* dispatcher_ { nullptr };
};

#endif
