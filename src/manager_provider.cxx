#include "manager_provider.hxx"

void j_manager_provider::startup() {
    is_running = true;

    resource->startup();
    env->startup(*resource);
}

void j_manager_provider::shutdown() {
    is_running = false;

    env->shutdown();
    resource->shutdown();
}

j_manager_provider::~j_manager_provider() {
    if (is_running) {
        shutdown();
    }
}
