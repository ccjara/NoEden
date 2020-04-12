#include "env_manager.hxx"

void env_manager::startup(resource_manager& resource) {
    root_config_ = std::make_unique<root_config>();
    root_config_->read(resource.load_file("config.cfg"));

    is_running_ = true;
}

void env_manager::shutdown() noexcept {
    
}

const root_config& env_manager::config() const noexcept {
    return *root_config_;
}

bool env_manager::is_running() const noexcept {
    return is_running_;
}

void env_manager::stop() noexcept {
    is_running_ = false;
}

clock_c& env_manager::clock() noexcept {
    return clock_;
}
