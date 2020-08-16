#include "engine.hxx"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    SDL_SetMainReady();

    el::Loggers::reconfigureAllLoggers(el::Configurations { "logger.cfg" });

    j_engine engine;

    engine.run();

    return 0;
}
