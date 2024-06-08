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
    Events* events
) : entity_reader_(entity_reader),
    action_creator_(action_creator),
    events_(events) {
    assert(events_);

    events_->engine->on<KeyDownEvent>(this, &GamePlayerController::on_key_down);
}

EventResult GamePlayerController::on_key_down(KeyDownEvent& e) {
    Vec2<i32> position = { 0, 0 };

    if (e.key == Key::W) {
        position.y = -1;
    } else if (e.key == Key::A) {
        position.x = -1;
    } else if (e.key == Key::S) {
        position.y = 1;
    } else if (e.key == Key::D) {
        position.x = 1;
    } else if (e.key == Key::Q) {
        position.x = -1;
        position.y = -1;
    } else if (e.key == Key::E) {
        position.x = 1;
        position.y = -1;
    } else if (e.key == Key::Z || e.key == Key::Y) {
        position.x = -1;
        position.y = 1;
    } else if (e.key == Key::C) {
        position.x = 1;
        position.y = 1;
    } else {
        return EventResult::Continue;
    }

    move_relative(position);
    return EventResult::Halt;
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
    events_->engine->trigger<PlayerActionCommitted>(move_action);

    return true;
}
