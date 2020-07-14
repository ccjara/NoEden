#include "env_manager.hxx"

void j_env_manager::startup(j_script_system& scripts) {
    auto& sys_script = scripts.require("system");

    root_config_ = std::make_unique<j_root_config>(sys_script);

    is_running_ = true;

    el::Configurations conf { "logger.cfg" };
    el::Loggers::reconfigureLogger("default", conf);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR) << "SDL could not initialize! (" << SDL_GetError() << ")";
        throw;
    }

    window_ = std::make_unique<j_window>(root_config_->resolution());
    max_threads_ = std::thread::hardware_concurrency();

    events_.dispatcher().sink<j_quit_event>().connect<&j_env_manager::on_quit_event>(this);
    events_.dispatcher().sink<j_resize_event>().connect <&j_env_manager::on_resize_event>(this);
}

void j_env_manager::shutdown() noexcept {
    window_.reset(); // free handle, RAII would destroy the window after SDL_QUIT

    SDL_Quit();
}

void j_env_manager::on_quit_event(const j_quit_event&) {
    is_running_ = false;
}

void j_env_manager::on_resize_event(const j_resize_event& e) {
    window_->resize(e.size);
}

const j_root_config& j_env_manager::config() const noexcept {
    return *root_config_;
}

bool j_env_manager::is_running() const noexcept {
    return is_running_;
}

void j_env_manager::stop() noexcept {
    is_running_ = false;
}

j_clock& j_env_manager::clock() noexcept {
    return clock_;
}

j_env_event_system& j_env_manager::events() noexcept {
    return events_;
}

j_window& j_env_manager::window() noexcept {
    return *window_;
}
