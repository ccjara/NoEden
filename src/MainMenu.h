#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

class MainMenu
{
private:
    std::shared_ptr<EventHandler> onKeyPressedEventHandler;
    void onKeyPressed(const SDL_Event& e);
public:
    MainMenu(EventManager& eventManager);
};

#endif
