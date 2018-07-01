#include "stdafx.h"

void MainMenu::onKeyPressed(const SDL_Event& e)
{
    LOG(INFO) << "KEY PRESSED IN MAINMENU " << e.key.keysym.scancode;
}

void MainMenu::attach(EventManager& eventManager)
{
    EventCallback callback = std::bind(&MainMenu::onKeyPressed, this, std::placeholders::_1);
    auto id = eventManager.subscribe(SDL_EventType::SDL_KEYDOWN, callback);
    eventManager.unsubscribe(SDL_EventType::SDL_KEYDOWN, id);
}