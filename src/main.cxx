#include "engine/engine.hxx"
#include "systems/system_factory_impl.hxx"
#include "scenes/scene_factory_impl.hxx"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    SDL_SetMainReady();

    system_factory_impl sys_factory;
    scene_factory_impl scene_factory;

    engine eng(&sys_factory, &scene_factory);

    eng.startup();
    eng.run();
    eng.shutdown();

    return 0;
}
