#include "api_registrar.hxx"

ApiRegistrar::ApiRegistrar(entt::dispatcher& dispatcher) : 
    dispatcher_ { dispatcher } {
    dispatcher_.sink<ScriptLoadedEvent>().connect<&ApiRegistrar::on_script_loaded>(this);
}

void ApiRegistrar::on_script_loaded(const ScriptLoadedEvent& e) {
    for (auto& api : apis_) {
        api->on_register(e.script);
    }
}
