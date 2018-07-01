#include "stdafx.h"
#undef main

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
    el::Configurations conf("logger.cfg");
    el::Loggers::reconfigureLogger("default", conf);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(ERROR) << "SDL could not initialize! (" << SDL_GetError() << ")";
        return 1;
    }

    Core core;
    core.run();

    SDL_Quit();

    return 0;
}
