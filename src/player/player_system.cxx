#include "player_system.hxx"

void j_player_system::on_load() {
    dispatcher_->sink<j_player_control_event>().connect<&j_player_system::on_player_control>(this);
    dispatcher_->sink<j_key_down_event>().connect<&j_player_system::on_key_down>(this);
}

void j_player_system::update(uint32_t delta_time) {
    if (player_ == entt::null) {
        return;
    }
    // handle movement
    auto& position { game->entities()->get<jc_position>(player_) };

    if (velocity_.x || velocity_.y) {
        if (static_cast<int32_t> (position.x) + velocity_.x >= 0) {
            position.x += velocity_.x;
        }
        if (static_cast<int32_t> (position.y) + velocity_.y >= 0) {
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

void j_player_system::on_player_control(const j_player_control_event& e) {
    player_ = e.player;
}

void j_player_system::on_key_down(const j_key_down_event& e) {
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
    }
}