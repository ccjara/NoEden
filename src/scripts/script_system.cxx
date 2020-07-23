#include "script_system.hxx"

void j_script_system::attach(entt::dispatcher& dispatcher) {
    script_env_.attach(dispatcher);
}
