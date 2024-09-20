#include "game/game.hxx"

static_assert(!LUABRIDGE_HAS_EXCEPTIONS);

int main() {
    return Game::start();
}
