#include "player_system.hxx"

void j_player_system::on_load() {
    events_->bind<j_player_control_event, &j_player_system::task_player_control>(this);
    events_->bind<KeyDownEvent, &j_player_system::task_key_down>(this);
}

void j_player_system::update(uint32_t delta_time) {
    events_->process();

    if (player_ == entt::null) {
        return;
    }
    // handle movement
    auto& position { game->entities()->get<jc_position>(player_) };

    if (velocity_.x || velocity_.y) {
        game->events()->trigger<j_world_step_event>(j_player_steps::move);

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

void j_player_system::task_key_down(const KeyDownEvent& e) {
    if (player_ == entt::null || !controlling_) {
        return;
    }
    switch (e.key) {
        case Key::Up:
            velocity_.y -= 1;
            break;
        case Key::Down:
            velocity_.y += 1;
            break;
        case Key::Left:
            velocity_.x -= 1;
            break;
        case Key::Right:
            velocity_.x += 1;
            break;
        case Key::G:
            gathering_ = true;
            break;
        case Key::I:
            auto& inv { game->entities()->get<jc_inventory>(player_) };
            game->events()->trigger<j_inventory_view_event>(player_, &inv);
    }
}
