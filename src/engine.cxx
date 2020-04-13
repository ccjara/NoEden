#include "engine.hxx"

void j_engine::run() {
    managers_.startup();

    gfx_system_ = std::make_unique<j_gfx_system>(&managers_.env->window());

    auto& clock { managers_.env->clock() };

    clock.start();

    while (managers_.env->is_running()) {
        clock.tick();

        managers_.env->dispatcher().listen();

        gfx_system_->temp__render();
    }

    managers_.shutdown();
}
