#include "game_factory.hxx"

void GameFactory::run() {
    auto game { std::make_unique<Game>() };

    game->start();
    //xrays_.emplace_back(std::make_unique<UiXray>());
    //xrays_.emplace_back(std::make_unique<ScriptXray>());
    game->xray_.add<SceneXray>(game->dispatcher_);
    game->xray_.add<ScriptXray>(game->dispatcher_, game->scripts_);
    game->xray_.add<UiXray>(game->dispatcher_, game->ui_);

    auto& player { game->scene_.create_actor(&Archetypes::Dwarf) };
    auto& troll { game->scene_.create_actor(&Archetypes::Troll) };

    // TODO: move as factory to archetype
    troll.ai.add<AiWalk>(0, &troll, game->action_queue_);

    player.position = { 0, 1 };
    troll.position = { 3, 3 };

    game->player_controller_.control(&player);
    game->run();
}
