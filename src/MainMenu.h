#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

class MainMenu
{
private:
    void onKeyPressed(const SDL_Event& e);
public:
    void attach(EventManager& eventManager);
};

#endif
