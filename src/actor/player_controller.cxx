#include "player_controller.hxx"

PlayerController::PlayerController(
    ActionQueue& queue,
    EventManager& events
) : queue_ { queue }, events_ { events } {
    events_.on<KeyDownEvent>(this, &PlayerController::on_key_press);
}

void PlayerController::control(Actor* actor) {
    player_ = actor;
}

Action* PlayerController::pull_player_action() {
    return std::exchange(player_action_, nullptr);
}

bool PlayerController::on_key_press(KeyDownEvent& e) {
    if (!player_) {
        return false;
    }
    auto walk_relative = [&](Vec2<i32> direction) {
        player_action_ = &queue_.push<WalkAction>(
            player_,
            player_->position + direction
        );
    };
    switch (e.key) {
        case Key::W:
            walk_relative(Vec2<i32> { 0, -1 });
            return true;
        case Key::A:
            walk_relative(Vec2<i32> { -1, 0 });
            return true;
        case Key::S:
            walk_relative(Vec2<i32> { 0, 1 });
            return true;
        case Key::D:
            walk_relative(Vec2<i32> { 1, 0 });
            return true;
        default:
            return false;
    }
}

Actor* PlayerController::player() {
    return player_;
}
