#include "env_manager.hxx"

void j_env_manager::startup(j_resource_loader& res_loader) {
    root_config_ = std::make_unique<j_root_config>();
    root_config_->read(res_loader.load_file("config.cfg"));

    is_running_ = true;

    el::Configurations conf { "logger.cfg" };
    el::Loggers::reconfigureLogger("default", conf);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR) << "SDL could not initialize! (" << SDL_GetError() << ")";
        throw;
    }

    window_ = std::make_unique<j_window>(root_config_->resolution());
    max_threads_ = std::thread::hardware_concurrency();

    dispatcher_.on<j_quit_event, &j_env_manager::on_quit_event>(this);
}

void j_env_manager::shutdown() noexcept {
    SDL_Quit();
}

void j_env_manager::on_quit_event(const j_quit_event&) {
    is_running_ = false;
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

j_env_event_dispatcher& j_env_manager::dispatcher() noexcept {
    return dispatcher_;
}

j_window& j_env_manager::window() noexcept {
    return *window_;
}
