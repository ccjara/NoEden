#include "player_system.hxx"

void j_player_system::on_load() {
    define_task<j_player_control_event, &j_player_system::task_player_control>();
    define_task<j_key_down_event, &j_player_system::task_key_down>();
}

void j_player_system::update(uint32_t delta_time) {
    queue_.update();

    if (player_ == entt::null) {
        return;
    }
    // handle movement
    auto& position { game->entities()->get<jc_position>(player_) };

    if (velocity_.x || velocity_.y) {
        if (position.x + velocity_.x >= 0) {
            position.x += velocity_.x;
        }
        if (position.y + velocity_.y >= 0) {
            position.y += velocity_.y;
        }
        velocity_ = { 0, 0 };
        game->events()->trigger<j_player_moved_event>(player_, position);
    }

    if (gathering_) {
        game->events()->trigger<j_gathering_started_event>(player_, position);
        gathering_ = false;
    }
}

void j_player_system::task_player_control(const j_player_control_event& e) {
    player_ = e.player;
    controlling_ = true;
}

void j_player_system::task_key_down(const j_key_down_event& e) {
    if (player_ == entt::null || !controlling_) {
        return;
    }
    switch (e.key) {
        case SDL_KeyCode::SDLK_UP:
            velocity_.y -= 1;
            break;
        case SDL_KeyCode::SDLK_DOWN:
            velocity_.y += 1;
            break;
        case SDL_KeyCode::SDLK_LEFT:
            velocity_.x -= 1;
            break;
        case SDL_KeyCode::SDLK_RIGHT:
            velocity_.x += 1;
            break;
        case SDL_KeyCode::SDLK_g:
            gathering_ = true;
            break;
        case SDL_KeyCode::SDLK_i:
            auto& inv { game->entities()->get<jc_inventory>(player_) };
            game->events()->trigger<j_inventory_view_event>(player_, &inv);
    }
}
