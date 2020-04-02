#include "engine/engine.hxx"
#include "scenes/scene_resolver_impl.hxx"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
    SDL_SetMainReady();

    const auto scenes { std::make_unique<scene_resolver_impl>() };

    engine eng(scenes.get());

    eng.startup();
    eng.run();
    eng.shutdown();

    return 0;
}
