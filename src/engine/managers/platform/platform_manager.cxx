#include "platform_manager.hxx"

uint32_t platform_manager::max_threads() const noexcept {
    return max_threads_;
}

void platform_manager::startup(const env_manager& env) {
    const auto& env_cfg { env.config() };
    el::Configurations conf{ "logger.cfg" };
    el::Loggers::reconfigureLogger("default", conf);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR) << "SDL could not initialize! (" << SDL_GetError() << ")";
        throw;
    }

    window_ = std::make_unique<window>(env_cfg.resolution());
    max_threads_ = std::thread::hardware_concurrency();
}

void platform_manager::shutdown() noexcept {
    SDL_Quit();
}

void platform_manager::process_events(env_manager& env) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            return env.stop();
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                window_->resize({
                    static_cast<uint32_t> (e.window.data1),
                    static_cast<uint32_t> (e.window.data2)
                });
            }
            break;
        case SDL_MOUSEBUTTONDOWN: {
            mouse_click_event event;
            event.button = static_cast<mouse_button>(e.button.button);
            events_->trigger<mouse_click_event>(event);
            break;
        }
        case SDL_MOUSEMOTION: {
            mouse_move_event event;
            event.pos = { e.motion.x, e.motion.y };
            events_->trigger<mouse_move_event>(event);
            break;
        }
        }
    }
}

const window& platform_manager::get_window() const noexcept {
    return *window_;
}
