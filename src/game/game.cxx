#include "game.hxx"

Game::Game() :
    input_ { events_ },
    renderer_ { window_, events_ },
    ui_ { events_, renderer_.display() },
    player_controller_ { action_queue_, events_ },
    scripting_ { events_ },
    xray_ { window_, events_ },
    action_queue_ { scene_ } {
}

void Game::start() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::error("SDL could not initialize: {}", SDL_GetError());
        std::abort();
    }

    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(0, &display_bounds);

    window_.open(Vec2<u32>{
        static_cast<u32>(static_cast<float>(display_bounds.w) / 1.25f),
        static_cast<u32>(static_cast<float>(display_bounds.h) / 1.25f)
    }, "Jaralyn");

    renderer_.initialize();

    events_.on<ScriptLoadedEvent>(this, &Game::on_script_loaded);

    is_running_ = true;
}

void Game::process_os_messages() {
    SDL_PumpEvents();

    SDL_Event e;
    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_FIRSTEVENT, SDL_EventType::SDL_WINDOWEVENT) != 0) {
        switch (e.type) {
        case SDL_EventType::SDL_QUIT:
            is_running_ = false;
            return;
        case SDL_EventType::SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                window_.resize(Vec2<u32> {
                    static_cast<u32> (e.window.data1),
                    static_cast<u32> (e.window.data2)
                });
                events_.trigger<ResizeEvent>(window_.size());
            }
            break;
        }
    }
    input_.poll_platform();
}

void Game::run() {
    ui_.startup();
    scripting_.startup();
    xray_.startup(renderer_.gl_context());

    events_.trigger<SceneLoadedEvent>(&scene_);

    scene_.update_fov(player_controller_.player());

    while (true) {
        process_os_messages();
        if (!is_running_) {
            break;
        }

        // world clock advances upon player commands
        if (auto player_action = player_controller_.pull_player_action()) {
            for (auto& actor : scene_.actors()) {
                actor->energy += player_action->cost;
                // TODO: ai must calculate number of possible actions, then push them
                //       the cost are deducted when performing the action so that
                //       if an actor gets impaired / slowed the action may fail in that cycle
                actor->ai.visit();
            }
            action_queue_.process();

            // update pov
            scene_.update_fov(player_controller_.player());
        }

        // update engine submodules
        ui_.update();

        // render
        renderer_.render(scene_);
    }

    xray_.shutdown();
    scripting_.shutdown();
    ui_.shutdown();

    window_.close();
    SDL_Quit();
}

bool Game::on_script_loaded(ScriptLoadedEvent& e) {
    luabridge::getGlobalNamespace(e.script->lua_state())
        .beginClass<Game>("Env")
            .addFunction("configure", &Game::configure_from_lua)
        .endClass();

    luabridge::setGlobal(e.script->lua_state(), this, "env");
    return false;
}

void Game::configure_from_lua(luabridge::LuaRef cfg) {
    Config cfg_prev = config_;

    constexpr const auto report = [](std::string_view reason) -> void {
        Log::error("Error in root config: {}.", reason);
    };

    if (!cfg.isTable()) {
        report("Expected payload to be a table");
        return;
    }

    // TODO: abstract runtime type checking and default value handling
    if (cfg["scaling"].isNumber()) {
        config_.scaling = cfg["scaling"].cast<float_t>();
    } else {
        report("Expected gfx:scaling to be a number");
    }
    if (cfg["font_texture_path"].isString()) {
        config_.font_texture_path = cfg["font_texture_path"].cast<std::string>();
    } else {
        report("Expected gfx:font_texture_path to be a string");
    }

    const auto& glyph_size { cfg["glyph_size"] };
    if (glyph_size.isTable()) {
        if (glyph_size["width"].isNumber()) {
            config_.glyph_size.x = glyph_size["width"];
        } else {
            report("Expected gfx:glyph_size:width to be a number");
        }
        if (glyph_size["height"].isNumber()) {
            config_.glyph_size.y = glyph_size["height"];
        } else {
            report("Expected gfx:glyph_size:height to be a number");
        }
    } else {
        report("Expected gfx:glyph_size to be a table");
    }
    events_.trigger<ConfigUpdatedEvent>(std::move(cfg_prev), config_);
}

bool Game::running() const {
    return is_running_;
}
