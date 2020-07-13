#include "engine.hxx"

void j_engine::run() {
    env_ = std::make_unique<j_env_manager>();
    script_system_ = std::make_unique<j_script_system>();

    // manager startup (keep in sync with shutdown reverse order)
    // TODO: clean up inconsistencies between systems and 'managers'
    env_->startup(*script_system_);

    gfx_system_ = std::make_unique<j_gfx_system>(&env_->window());
    input_system_ = std::make_unique<j_input_system>();

    // wire event listeners
    auto& dispatcher { env_->events().dispatcher() };
    gfx_system_->attach(dispatcher);
    input_system_->attach(dispatcher);

    // testing: directly load world for now
    composer_.load(j_scene_type::world);

    while (env_->is_running()) {
        // update
        env_->events().listen();
        env_->clock().tick([&]() { composer_.update(*input_system_); });
        // render
        gfx_system_->prepare();
        composer_.render(gfx_system_->display());
        gfx_system_->present();
    }

    // manager shutdown (keep in sync with startup reverse order)
    env_->shutdown();
}
