#include "stdafx.h"

Game::Game() :
    mainMenu(sdlEventManager)
{
}

void Game::handleSdlEvent(const SDL_Event& e)
{
    if (e.type == SDL_MOUSEMOTION) {
        return;
    }
    sdlEventManager.trigger(e);
}
