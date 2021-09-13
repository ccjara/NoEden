#include "game/game_factory.hxx"

static_assert(!LUABRIDGE_HAS_EXCEPTIONS);

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    GameFactory::run();
    return 0;
}
