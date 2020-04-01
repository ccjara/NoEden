#include "engine.hxx"

void engine::startup() noexcept {
    resource_->startup();
    env_->startup(*resource_);
    platform_->startup(*env_);
    task_->startup(*platform_);
}

void engine::shutdown() noexcept {
    task_->shutdown();
    platform_->shutdown();
    env_->shutdown();
    resource_->shutdown();
}

void engine::run() {
    while (env_->is_running()) {
        platform_->process_events(*env_);

        task_->process(systems_);
    }
}

void engine::register_system(game_system* sys) {
    sys->attach(*platform_->events_);

    systems_.push_back(sys);

    sys->on_load(managers_);
}