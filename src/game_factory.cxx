#include "game_factory.hxx"

j_game* game { nullptr };

void j_game_factory::run() {
    game = new j_game();
    if (!game) {
        LOG(ERROR) << "Could not allocate game memory";
        throw;
    }
    game->state_ = std::make_unique<j_state_stack>();

    game->env_->start();
    game->systems_->emplace<j_input_system>();
    game->systems_->emplace<j_player_system>();
    game->systems_->emplace<j_gfx_system>(&game->env_->window());
    game->systems_->emplace<j_hud_system>();
    game->systems_->emplace<j_script_system>();

    game->run();

    delete game;
}