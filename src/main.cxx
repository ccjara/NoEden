#include "game/game_factory.hxx"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    el::Loggers::reconfigureAllLoggers(el::Configurations { "logger.cfg" });

    GameFactory::run();
    return 0;
}
