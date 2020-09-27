#include "game_factory.hxx"

j_game* game { nullptr };

void j_game_factory::run() {
    game = new j_game();
    if (!game) {
        LOG(ERROR) << "Could not allocate game memory";
        std::abort();
    }
    game->env_->start();
    game->systems_->emplace<j_input_system>();
    game->systems_->emplace<j_player_system>();
    game->systems_->emplace<j_unit_system>();
    game->systems_->emplace<j_hud_system>();
    game->systems_->emplace<j_state_system>();
    game->systems_->emplace<j_gfx_system>(&game->env_->window());
    // FIXME: emit default root_config before the script system is loaded so
    //        the gfx system can startup independently...
    game->systems_->emplace<j_script_system>();

    game->run();

    delete game;
}
