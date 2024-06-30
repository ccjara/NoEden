#include "world/player_controller.hxx"
#include "action/action_queue.hxx"
#include "action/action_event.hxx"
#include "action/move_action.hxx"
#include "entity/entity.hxx"
#include "entity/entity_manager.hxx"
#include "world/world_context.hxx"

void PlayerController::initialize(WorldContext* world_context) {
    assert(world_context);
    world_context_ = world_context;

    key_down_sub_ = world_context_->events->on<KeyDownEvent>(this, &PlayerController::on_key_down);
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
    Entity* player = world_context_->entity_manager->player();
    if (!player) {
        return false;
    }
    auto result = world_context_->action_queue->create_action(ActionType::Move, *player);
    if (result.failed()) {
        return false;
    }
    auto move_action = static_cast<MoveAction*>(result.action);
    move_action->destination = player->position + direction;

    // start the next world tick
    world_context_->events->trigger<PlayerActionCommitted>(move_action);
    return true;
}
