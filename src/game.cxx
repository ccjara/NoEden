#include "game.hxx"

j_game::j_game() {
    env_ = std::make_unique<j_env_manager>(&dispatcher_);
    systems_ = std::make_unique<j_systems>(&dispatcher_);
    input_ = std::make_unique<Input>(dispatcher_);
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

j_env_manager& j_game::env() {
    return *env_;
}

void j_game::run() {
    while (true) {
        env_->process_os_messages();
        input_->poll_platform();

        if (!env_->running()) {
            break;
        }
        env_->clock().tick([&](std::chrono::milliseconds delta_time) {
            systems_->update(delta_time.count());
        });
    }
}
