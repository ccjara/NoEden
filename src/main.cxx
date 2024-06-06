#include "game/game.hxx"

static_assert(!LUABRIDGE_HAS_EXCEPTIONS);

int main(int argc, char* argv[]) {
    return Game::start();
}
