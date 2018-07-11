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

    Vector2<GLfloat> p{ 10.0f, 10.0f };

    // TODO: improve readability, e.g. "The [c:ff0000ff]quick[/] ..."
    renderer->text->begin();
    std::string text("The |cff0000ffquick |c00ff00fffox|r jumps |cffff00ffover|r the lazy dog");
    renderer->text->renderText(text, p);
    text = std::string("The |cffffff22quick|r fox jumps over the |cffffff22lazy|r dog");
    p.y += 30;
    renderer->text->renderText(text, p);
    renderer->text->end();

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
