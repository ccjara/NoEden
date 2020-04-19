#include "engine.hxx"

void j_engine::run() {
    managers_.startup();

    gfx_system_ = std::make_unique<j_gfx_system>(&managers_.env->window());
    input_system_ = std::make_unique<j_input_system>();
    input_system_->attach(managers_.env->events().dispatcher());

    auto& clock { managers_.env->clock() };

    composer_.load(j_scene_type::world);

    while (managers_.env->is_running()) {
        managers_.env->events().listen();

        clock.tick([&]() { composer_.update(*input_system_); });

        gfx_system_->prepare();

        composer_.render(gfx_system_->display());

        gfx_system_->present();
    }

    managers_.shutdown();
}
