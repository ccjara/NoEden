#include "game_factory.hxx"

void GameFactory::run() {
    Log::startup();

    auto game { std::make_unique<Game>() };
    // xray / engine ui
    game->xray_.add<LogXray>();
    game->xray_.add<SceneXray>(game->events_, game->player_controller_, game->renderer_);
    game->xray_.add<ScriptXray>(game->events_, game->scripting_);
    game->xray_.add<UiXray>(game->events_, game->ui_);
    
    // scripting
    auto& lua_registrar { game->scripting_.registrar() };
    lua_registrar.add_api<SceneApi>(game->scene_);
    lua_registrar.add_api<UiApi>(game->ui_.ui_tree());

    game->start();

    auto& player { game->scene_.create_actor(&Archetypes::Dwarf) };
    auto& troll { game->scene_.create_actor(&Archetypes::Troll) };

    // TODO: move as factory to archetype
    troll.ai.add<AiWalk>(0, &troll, game->action_queue_);

    player.position = { 0, 1 };
    troll.position = { 3, 3 };

    game->player_controller_.control(&player);
    game->run();
}
