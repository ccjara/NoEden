#include "scripts/api/game_api.hxx"
#include "game/exit_manager.hxx"

void GameApi::on_register(Script& script) {
    luabridge::getGlobalNamespace(script)
        .beginClass<GameApi>("GameApi")
            .addFunction("exit", &GameApi::exit)
        .endClass();

    script.define_enum("Key", std::make_tuple("Escape", SDLK_ESCAPE));

    expose(script, this, "game");
}

bool GameApi::exit(bool force) const {
    return svc_->get<ExitManager>()->request_exit(force);
}
