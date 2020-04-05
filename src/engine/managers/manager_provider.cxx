#include "manager_provider.hxx"

void manager_provider::startup() {
    is_running = true;

    resource->startup();
    env->startup(*resource);
    platform->startup(this);
    task->startup(*platform);
    state->startup();
}

void manager_provider::shutdown() {
    is_running = false;

    state->shutdown();
    task->shutdown();
    platform->shutdown();
    env->shutdown();
    resource->shutdown();
}

manager_provider::~manager_provider() {
    if (is_running) {
        shutdown();
    }
}