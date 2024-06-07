#ifndef NOEDEN_CONFIG_API_HXX
#define NOEDEN_CONFIG_API_HXX

#include "scripts/api/lua_api.hxx"

class ConfigManager;

class ConfigApi : public LuaApi {
public:
    explicit ConfigApi(ConfigManager* config_manager, Events* events);

    void on_register(Script& script) override;
private:
    /**
     * @brief Updates the config from inside a lua script
     */
    void on_configure(luabridge::LuaRef cfg);

    ConfigManager* config_manager_ = nullptr;
    Events* events_ = nullptr;
};

#endif
