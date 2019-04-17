#include "stdafx.h"

Game::Game() :
    mainMenu(sdlEventManager),
    test{ std::make_unique<TextTester>(sdlEventManager) }
{
}

void Game::handleSdlEvent(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEMOTION) {
        return;
    }
    sdlEventManager.trigger(e);
}

void Game::render(const Renderer& renderer)
{
    Vector2<GLfloat> p{ 10.0f, 10.0f };

    // TODO: improve readability, e.g. "The [c:ff0000ff]quick[/] ..."
    renderer.text->begin();

    /*
    std::string text(
    "The |cff0000ffquick |c00ff00fffox|r jumps |cffff00ffover|r the lazy dog|r|n\
    The |cffffff22clunky|r cat collides with the |cffffff22fat|r boar|n\
    This|nText|n|n|nHas |c00ff00ff4|r newlines in total.|n\
    |cff0000ffHey||Pipe"
    );
    */
    renderer.text->renderText(test->getText(), p);
    renderer.text->end();
}