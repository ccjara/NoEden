#ifndef NOEDEN_SCRIPT_API_HXX
#define NOEDEN_SCRIPT_API_HXX

#include "scripts/api/lua_api.hxx"

class Scripting;

/**
 * @brief API to interact with scripting functionality, such as reacting to script events.
 */
class ScriptApi : public LuaApi {
public:
    bool initialize() override;

    void on_register(Script& script) override;
private:
    /**
     * @brief Runs a script by name
     */
    bool run(const char* name);

    Scripting* scripting_ = nullptr;
};
#endif
