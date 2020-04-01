#include "engine/engine.hxx"
#include "systems/gfx/gfx_system.hxx"
#include "systems/input/input_system.hxx"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    SDL_SetMainReady();

    input_system input;
    gfx_system gfx;

    engine eng;

    eng.startup();
    eng.register_system(&input);
    eng.register_system(&gfx);
    eng.run();
    eng.shutdown();

    return 0;
}
