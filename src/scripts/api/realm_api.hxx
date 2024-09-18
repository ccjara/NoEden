#ifndef NOEDEN_REALM_API_HXX
#define NOEDEN_REALM_API_HXX

#include "scripts/api/lua_api.hxx"
#include "realm/realm_event.hxx"

class RealmManager;
class Scripting;

class RealmApi : public LuaApi {
public:
    bool initialize() override;

    void on_register(Script& script) override;
private:
    /**
     * @brief Registers a callback to be called when a realm is loaded
     *
     * @param callback Lua function to call when the realm is loaded
     */
    void on_load(luabridge::LuaRef callback);

    /**
     * @brief Provides the current realm type to a lua script
     *
     * @return const char* Identifier of the currently active realm
     */
    i32 current();

    RealmManager* realm_manager_ = nullptr;
    Scripting* scripting_ = nullptr;
    Subscription<RealmLoadedEvent> realm_loaded_sub_;

    EventResult on_realm_loaded(const RealmLoadedEvent& e);
};

#endif
