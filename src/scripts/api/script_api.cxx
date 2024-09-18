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
            .addFunction("subscribe", &ScriptApi::subscribe)
        .endClass();

    script.define_enum("EventType", std::make_tuple("KeyDown", EventType::KeyDown));

    expose(script, this, "script");
}

bool ScriptApi::run(const char* name) { return scripting_->run(name); }

bool ScriptApi::subscribe(i32 eventTypeRaw, luabridge::LuaRef callback) {
    const auto event_type{static_cast<EventType>(eventTypeRaw)};

    switch (event_type) {
    case EventType::KeyDown:
        scripting_->add_callback(event_type, callback);
        return true;
    default:
        LOG_ERROR("Could not subscribe to unrecognized event type {}", eventTypeRaw);
        return false;
    }

    scripting_->add_callback(event_type, callback);
}
