#include "world/game_player_controller.hxx"
#include "action/action_creator.hxx"
#include "action/action_event.hxx"
#include "action/move_action.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"
#include "input/input_event.hxx"

GamePlayerController::GamePlayerController(
    IEntityReader* entity_reader,
    IActionCreator* action_creator,
    EventManager* events
) : entity_reader_(entity_reader),
    action_creator_(action_creator),
    events_(events) {
    assert(events_);

    events_->on<KeyDownEvent>(this, &GamePlayerController::on_key_down);
}

bool GamePlayerController::on_key_down(KeyDownEvent& e) {
    switch (e.key) {
        case Key::W:
            return move_relative(Vec2<i32>{ 0, -1 });
        case Key::A:
            return move_relative(Vec2<i32>{ -1, 0 });
        case Key::S:
            return move_relative(Vec2<i32>{ 0, 1 });
        case Key::D:
            return move_relative(Vec2<i32>{ 1, 0 });
        default:
            return false;
    }
}

bool GamePlayerController::move_relative(const Vec2<i32>& direction) {
    Entity* player = entity_reader_->player();
    if (!player) {
        return false;
    }
    auto result = action_creator_->create_action(ActionType::Move, *player);
    if (result.failed()) {
        return false;
    }
    auto move_action = static_cast<MoveAction*>(result.action);
    move_action->destination = player->position + direction;

    // start the next world tick
    events_->trigger<PlayerActionCommitted>(move_action);

    return true;
}
