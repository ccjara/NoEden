#ifndef NOEDEN_SCRIPT_API_HXX
#define NOEDEN_SCRIPT_API_HXX

#include "scripts/api/lua_api.hxx"

/**
 * @brief API to interact with scripting functionality, such as reacting to script events.
 */
class ScriptApi : public LuaApi {
public:
    void on_register(Script& script) override;
};
#endif
