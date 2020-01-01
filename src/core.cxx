#include "core.hxx"

core::core() :
    renderer_ptr(std::make_unique<renderer>()),
    game_ptr(std::make_unique<game>()),
    window_ptr(std::make_shared<window>(window_size { 800, 600 })) {

    renderer_ptr->bind(window_ptr);
}

void core::process_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return shutdown();
        }

        game_ptr->handle_sdl_event(e);

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            window_ptr->resize({
                static_cast<uint32_t> (e.window.data1),
                static_cast<uint32_t> (e.window.data2)
            });
        }
    }
}

void core::render() {
    renderer_ptr->start_rendering();

    game_ptr->render(*renderer_ptr);

    renderer_ptr->finish_rendering();
}

void core::run() {
    is_running = true;

    while (is_running) {
        process_events();
        render();
    }
}

void core::shutdown() noexcept {
    is_running = false;
}
