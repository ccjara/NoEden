#include "scripts/api/script_api.hxx"
#include "scripts/scripting.hxx"

bool ScriptApi::initialize() {
    scripting_ = svc_->get<Scripting>();
    return !!scripting_;
}

void ScriptApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<ScriptApi>("Script")
            .addFunction("run", &ScriptApi::run)
        .endClass();

    expose(script, this, "script");
}

bool ScriptApi::run(const char* name) {
    return scripting_->run(name);
}
