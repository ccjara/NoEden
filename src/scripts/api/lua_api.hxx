#ifndef NOEDEN_LUA_API
#define NOEDEN_LUA_API

#include "scripts/script.hxx"

/**
 * @brief Interface for lua api implementations.
 */
class LuaApi {
public:
    virtual ~LuaApi() = default;

    /**
     * @brief Called for each loaded script.
     *
     * Namespaces and globals for each script should be setup here.
     */
    virtual void on_register(Script& script) = 0;

    /**
     * @brief Initializes the api.
     *
     * Called after adding it to the api container.
     * `svc_` and `events_` are set before initialize is called.
     *
     * @return True if the api was successfully initialized.
     */
    virtual bool initialize() {
        return true;
    }

    /**
     * @brief Exposes the given api to lua under the given name.
     */
    template<typename Api>
    void expose(Script& script, Api* api, const char* name) {
        std::error_code ec;
        luabridge::push<Api*>(script, api, ec);
        if (ec) {
            LOG_ERROR("Could not expose api {}: {}", name, ec.message());
            return;
        }
        lua_setglobal(script, name);
    }
protected:
    friend class Scripting;
    ServiceLocator* svc_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
