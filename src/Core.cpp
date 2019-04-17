#include "stdafx.h"

Core::Core() :
    running{ false },
    renderer{ std::make_unique<Renderer>() },
    game{ std::make_unique<Game>() }
{
    WindowSize windowSize { 800, 600 };
    window = std::make_shared<Window>(windowSize);
    renderer->bind(window);
}

Core::~Core()
{
}

void Core::processEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return shutdown();
        }

        game->handleSdlEvent(e);

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            WindowSize s = {
                static_cast<WindowSizeType> (e.window.data1),
                static_cast<WindowSizeType> (e.window.data2)
            };
            window->adjust(s);
        }
    }
}

void Core::render()
{
    renderer->startRendering();

    game->render(*renderer);

    renderer->finishRendering();
}

void Core::run()
{
    running = true;

    while (running) {
        processEvents();
        render();
    }
}

void Core::shutdown() noexcept
{
    running = false;
}
