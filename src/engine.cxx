#include "engine.hxx"

void j_engine::run() {
    managers_.startup();

    gfx_system_ = std::make_unique<j_gfx_system>(&managers_.env->window());

    auto& clock { managers_.env->clock() };

    clock.start();

    composer_.load<j_world_scene>();

    while (managers_.env->is_running()) {
        clock.tick();

        managers_.env->dispatcher().listen();

        composer_.active().update();

        gfx_system_->prepare();

        composer_.active().render(gfx_system_->display());

        gfx_system_->present();
    }

    managers_.shutdown();
}
