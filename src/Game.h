#ifndef __GAME_H__
#define __GAME_H__

class Game
{
private:
    EventManager sdlEventManager;

    MainMenu mainMenu;
    std::unique_ptr<TextTester> test;
public:
    Game();
    void handleSdlEvent(const SDL_Event& e);
    void render(const Renderer& renderer);
};

#endif
