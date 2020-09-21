#include "game_factory.hxx"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    SDL_SetMainReady();
    el::Loggers::reconfigureAllLoggers(el::Configurations { "logger.cfg" });

    j_game_factory::run();
    return 0;
}
