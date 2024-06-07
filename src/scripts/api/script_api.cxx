#include "scripts/api/script_api.hxx"
#include "scripts/script.hxx"

void ScriptApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<ScriptApi>("Script")
        .endClass();
}
