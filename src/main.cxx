#include "game.hxx"

INITIALIZE_EASYLOGGINGPP

j_game* game { nullptr };

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    el::Loggers::reconfigureAllLoggers(el::Configurations { "logger.cfg" });

    const auto game_janitor { std::make_unique<j_game>() };
    if (!game_janitor) {
        return 1;
    }

    game = game_janitor.get();
    game->run();

    return 0;
}
