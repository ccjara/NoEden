#include "stdafx.h"

Game::Game()
{
    mainMenu.attach(sdlEventManager);
}

void Game::handleSdlEvent(const SDL_Event& e)
{
    sdlEventManager.trigger(e);
}
