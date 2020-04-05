#include "platform_manager.hxx"

uint32_t platform_manager::max_threads() const noexcept {
    return max_threads_;
}

void platform_manager::startup(manager_provider* const managers) {
    assert(managers);

    managers_ = managers;

    const auto& env_cfg { managers_->env->config() };
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

void platform_manager::assign_system_factory(system_factory* sys_factory) {
    assert(sys_factory);
    sys_factory_ = sys_factory;
}

const game_system& platform_manager::load_system(system_id_t id) {
    if (systems_.find(id) != systems_.end()) {
        LOG(ERROR) << "System " << id << " is already loaded";
        throw;
    }
    if (!sys_factory_) {
        LOG(ERROR) << "No system factory assigned";
        throw;
    }
    auto sys { sys_factory_->create(id, *managers_) };

    sys->attach(*events_);

    systems_[id] = std::move(sys);

    return *systems_[id];
}

void platform_manager::process_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_QUIT:
            return managers_->env->stop();
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

const system_map_t& platform_manager::systems() const noexcept {
    return systems_;
}