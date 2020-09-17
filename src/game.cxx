#include "game.hxx"

j_game::j_game() {
    env_ = std::make_unique<j_env_manager>(&dispatcher_);
    env_->attach(dispatcher_); // TODO: merge with constructor?
    systems_ = std::make_unique<j_systems>(&dispatcher_);
}

j_systems* j_game::systems() {
    return systems_.get();
}

entt::registry* j_game::entities() {
    return &entities_;
}

entt::dispatcher* j_game::events() {
    return &dispatcher_;
}

void j_game::run() {
    auto script_system_ = std::make_unique<j_script_system>(&dispatcher_);
    script_system_->attach(dispatcher_);

    env_->start();

    systems_->emplace<j_input_system>();
    systems_->emplace<j_player_system>();
    systems_->emplace<j_gfx_system>(&env_->window());

    script_system_->preload(script_system_->default_script_path);

    state_.push(j_state_id::world);

    while (true) {
        // update
        env_->process_os_messages();
        if (!env_->running()) {
            break;
        }

        env_->clock().tick([&](std::chrono::milliseconds delta_time) {
            systems_->update(delta_time.count());
        });
        
        state_.update();
    }
}