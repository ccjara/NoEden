#include "api_registrar.hxx"

ApiRegistrar::ApiRegistrar(EventManager& events) :
    events_ { events } {
    events_.on<ScriptLoadedEvent>(this, &ApiRegistrar::on_script_loaded);
}

bool ApiRegistrar::on_script_loaded(ScriptLoadedEvent& e) {
    for (auto& api : apis_) {
        api->on_register(e.script);
    }
    return false;
}
