#include "game/game.hxx"

static_assert(!LUABRIDGE_HAS_EXCEPTIONS);

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    Game game;
    game.run();
    return 0;
}
