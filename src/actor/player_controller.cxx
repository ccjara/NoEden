#include "player_controller.hxx"

PlayerController::PlayerController(
    ActionQueue& queue,
    entt::dispatcher& dispatcher
) : queue_ { queue }, dispatcher_ { dispatcher } {
    dispatcher_.sink<KeyDownEvent>().connect<&PlayerController::on_key_press>(this);
}

void PlayerController::control(Actor* actor) {
    player_ = actor;
}

Action* PlayerController::pull_player_action() {
    return std::exchange(player_action_, nullptr);
}

void PlayerController::on_key_press(const KeyDownEvent& e) {
    if (!player_) {
        return;
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
            break;
        case Key::A:
            walk_relative(Vec2<i32> { -1, 0 });
            break;
        case Key::S:
            walk_relative(Vec2<i32> { 0, 1 });
            break;
        case Key::D:
            walk_relative(Vec2<i32> { 1, 0 });
            break;
    }
}

Actor* PlayerController::player() {
    return player_;
}
