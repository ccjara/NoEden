#ifndef __GAME_H__
#define __GAME_H__

class Game
{
private:
    EventManager sdlEventManager;

    MainMenu mainMenu;
public:
    Game();
    void handleSdlEvent(const SDL_Event& e);
};

#endif
