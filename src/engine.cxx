#include "engine.hxx"

void j_engine::run() {
    managers_.startup();

    gfx_system_ = std::make_unique<j_gfx_system>(&managers_.env->window());
    input_system_ = std::make_unique<j_input_system>();
    input_system_->attach(managers_.env->events().dispatcher());

    auto& clock { managers_.env->clock() };

    composer_.load<j_world_scene>();

    while (managers_.env->is_running()) {
        managers_.env->events().listen();

        clock.tick([&]() { composer_.active().update(*input_system_); });

        gfx_system_->prepare();

        composer_.active().render(gfx_system_->display());

        gfx_system_->present();
    }

    managers_.shutdown();
}
