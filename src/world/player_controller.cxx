#include "world/player_controller.hxx"
#include "action/action_creator.hxx"
#include "action/action_event.hxx"
#include "action/move_action.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"

PlayerController::PlayerController(
    IEntityReader* entity_reader,
    IActionCreator* action_creator,
    EventManager* events
) : entity_reader_(entity_reader),
    action_creator_(action_creator),
    events_(events) {
    assert(entity_reader_);
    assert(action_creator_);
    assert(events_);

    key_down_sub_ = events_->on<KeyDownEvent>(this, &PlayerController::on_key_down);
}

EventResult PlayerController::on_key_down(KeyDownEvent& e) {
    WorldPos position;

    if (e.key == Key::W) {
        position.z = -1;
    } else if (e.key == Key::A) {
        position.x = -1;
    } else if (e.key == Key::S) {
        position.z = 1;
    } else if (e.key == Key::D) {
        position.x = 1;
    } else if (e.key == Key::Q) {
        position.x = -1;
        position.z = -1;
    } else if (e.key == Key::E) {
        position.x = 1;
        position.z = -1;
    } else if (e.key == Key::Z || e.key == Key::Y) {
        position.x = -1;
        position.z = 1;
    } else if (e.key == Key::C) {
        position.x = 1;
        position.z = 1;
    } else {
        return EventResult::Continue;
    }

    move_relative(position);
    return EventResult::Halt;
}

bool PlayerController::move_relative(const WorldPos& direction) {
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
