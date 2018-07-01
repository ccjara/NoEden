#include "stdafx.h"

MainMenu::MainMenu(EventManager& eventManager) :
    onKeyPressedEventHandler(
        std::make_shared<EventHandler>(
            SDL_KEYDOWN,
            std::bind(&MainMenu::onKeyPressed, this, std::placeholders::_1)
        )
    )
{
    eventManager.attach(onKeyPressedEventHandler);
}

void MainMenu::onKeyPressed(const SDL_Event& e)
{
    LOG(INFO) << "KEY PRESSED IN MAINMENU " << e.key.keysym.scancode;
}
