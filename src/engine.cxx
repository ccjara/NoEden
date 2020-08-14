#include "engine.hxx"

void j_engine::on_key_down(const j_key_down_event& e) {
    if (e.key == SDLK_F5) {
        script_system_->reload("system");
    }
}

void j_engine::run() {
    entt::dispatcher dispatcher;

    env_ = std::make_unique<j_env_manager>(&dispatcher);
    script_system_ = std::make_unique<j_script_system>(&dispatcher);
    gfx_system_ = std::make_unique<j_gfx_system>();
    input_system_ = std::make_unique<j_input_system>();
    composer_ = std::make_unique<j_scene_composer>(&dispatcher);

    // wire event listeners
    env_->attach(dispatcher);
    gfx_system_->attach(dispatcher);
    input_system_->attach(dispatcher);
    script_system_->attach(dispatcher);
    // testing: script reloading
    dispatcher.sink<j_key_down_event>().connect<&j_engine::on_key_down>(this);

    env_->start();

    // testing: directly load world for now
    composer_->load(j_scene_type::world);

    script_system_->preload("./scripts");

    while (true) {
        // update
        env_->process_os_messages();
        if (!env_->running()) {
            break;
        }
        env_->clock().tick([&]() { composer_->update(*input_system_); });
        // render
        gfx_system_->prepare();
        composer_->render(gfx_system_->display());
        gfx_system_->present();
    }
}
