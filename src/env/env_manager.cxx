#include "env_manager.hxx"

j_env_manager::j_env_manager(entt::dispatcher* const dispatcher) :
    dispatcher_(dispatcher) {
    assert(dispatcher_);
    env_event_dispatcher_ = std::make_unique<j_env_event_dispatcher>(
        dispatcher_
    );
}

j_env_manager::~j_env_manager() noexcept {
    if (is_running_) {
        stop();
    }
}

void j_env_manager::start() {
    el::Configurations conf { "logger.cfg" };
    el::Loggers::reconfigureLogger("default", conf);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR) << "SDL could not initialize! (" << SDL_GetError() << ")";
        throw;
    }

    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(0, &display_bounds);

    window_ = std::make_unique<j_window>(j_vec2<uint32_t>{
        static_cast<uint32_t>(display_bounds.w) / 2,
        static_cast<uint32_t>(display_bounds.h) / 2
    });

    dispatcher_->trigger<j_window_created_event>(window_.get());

    is_running_ = true;
}

void j_env_manager::on_quit(const j_quit_event&) {
    stop();
}

void j_env_manager::on_resize(const j_resize_event& e) {
    window_->resize(e.size);
}

void j_env_manager::on_script_loaded(const j_script_loaded_event& e) {
    if (e.script->id() == "system") {
        update_root_config(*e.script);
    }
}

void j_env_manager::update_root_config(j_script& sys_script) noexcept {
    j_root_config cfg_prev = root_config_;

    constexpr const auto report = [](std::string_view reason) noexcept -> void {
        LOG(ERROR) << "Error in root config: " << reason << ".";
    };

    if (sys_script.callable() && !sys_script.run()) {
        report("Runtime error during script execution");
        return;
    }
    if (!sys_script.called()) {
        report("System script is not loaded");
        return;
    }
    // TODO: abstract runtime type checking and default value handling
    const auto gfx_cfg = luabridge::getGlobal(sys_script, "gfx");
    if (gfx_cfg.isTable()) {
        if (gfx_cfg["scaling"].isNumber()) {
            root_config_.scaling = gfx_cfg["scaling"].cast<float_t>();
        } else {
            report("Expected gfx:scaling to be a number");
        }
        if (gfx_cfg["font_texture_path"].isString()) {
            root_config_.font_texture_path = gfx_cfg["font_texture_path"].cast<std::string>();
        } else {
            report("Expected gfx:font_texture_path to be a string");
        }

        const auto& glyph_size { gfx_cfg["glyph_size"] };
        if (glyph_size.isTable()) {
            if (glyph_size["width"].isNumber()) {
                root_config_.glyph_size.x = glyph_size["width"];
            } else {
                report("Expected gfx:glyph_size:width to be a number");
            }
            if (glyph_size["height"].isNumber()) {
                root_config_.glyph_size.y = glyph_size["height"];
            } else {
                report("Expected gfx:glyph_size:height to be a number");
            }
        } else {
            report("Expected gfx:glyph_size to be a table");
        }
    } else {
        report("Expected 'gfx' global to be a table");
    }
    dispatcher_->trigger(j_root_config_updated_event(std::move(cfg_prev), root_config_));
}

const j_root_config& j_env_manager::config() const noexcept {
    return root_config_;
}

bool j_env_manager::running() const noexcept {
    return is_running_;
}

void j_env_manager::stop() noexcept {
    window_->close();
    SDL_Quit();
    is_running_ = false;
}

j_clock& j_env_manager::clock() noexcept {
    return clock_;
}

j_window& j_env_manager::window() noexcept {
    return *window_;
}

void j_env_manager::process_os_messages() const noexcept {
    env_event_dispatcher_->listen();
}

void j_env_manager::attach(entt::dispatcher& dispatcher) noexcept {
    dispatcher_->sink<j_quit_event>().connect<&j_env_manager::on_quit>(this);
    dispatcher_->sink<j_resize_event>().connect<&j_env_manager::on_resize>(this);
    dispatcher_->sink<j_script_loaded_event>().connect<&j_env_manager::on_script_loaded>(this);
}
