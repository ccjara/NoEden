#include "ai/ai_walk.hxx"
#include "action/action_queue.hxx"
#include "action/move_action.hxx"
#include "entity/entity.hxx"
#include "entity/entity_manager.hxx"
#include "realm/realm.hxx"

void AiWalk::initialize(AiContext& context) {
    auto& realm_services = context.entity->realm->services();
    action_queue_ = realm_services.get<ActionQueue>();
    entity_manager_ = realm_services.get<EntityManager>();

    assert(action_queue_);
    assert(entity_manager_);
}

void AiWalk::clear() {
    mod_state(AiNodeState::Ready);
    target_type_ = WalkTargetType::Random;
    entity_target_key_ = default_target_key;
}

AiNodeState AiWalk::visit(AiContext& context) {
    if (target_type_ == WalkTargetType::Entity) {
        return perform_walk_to_entity(context);
    } else {
        return perform_walk_around(context);
    }
}

AiNodeState AiWalk::perform_walk_to_entity(AiContext& context) {
    assert(context.entity);
    auto target_id = context.blackboard.get<Id>(entity_target_key_);
    if (!target_id) {
        return mod_state(AiNodeState::Failed);
    }
    auto target = entity_manager_->entity(*target_id);
    if (!target) {
        return mod_state(AiNodeState::Failed);
    }

    auto create_action_result = action_queue_->create_action(ActionType::Move, *context.entity);

    if (create_action_result.failed()) {
        return mod_state(AiNodeState::Failed);
    }

    auto position = context.entity->position;
    const auto target_position = target->position;

    if (position.x < target_position.x) {
        ++position.x;
    } else if (position.x > target_position.x) {
        --position.x;
    }
    if (position.y < target_position.y) {
        ++position.y;
    } else if (position.y > target_position.y) {
        --position.y;
    }

    auto action = static_cast<MoveAction*>(create_action_result.action);
    action->destination = position;

    return mod_state(AiNodeState::Failed); // TODO: this must be changed later when repeatable actions (by conditions) are implemented
}

AiNodeState AiWalk::perform_walk_around(AiContext& context) {
    auto create_action_result = action_queue_->create_action(ActionType::Move, *context.entity);

    if (create_action_result.failed()) {
        return mod_state(AiNodeState::Failed);
    }

    auto pos = context.entity->position;

    static int dir = 0;
    if (dir == 0) {
        --pos.y;
    } else if (dir == 1) {
        ++pos.x;
    } else if (dir == 2) {
        ++pos.y;
    } else if (dir == 3) {
        --pos.x;
    }
    ++dir %= 4;

    auto action = static_cast<MoveAction*>(create_action_result.action);
    action->destination = pos;

    return mod_state(AiNodeState::Succeeded);
}

void AiWalk::target_entity(const std::string& entity_target_key) {
    target_type_ = WalkTargetType::Entity;
    entity_target_key_ = entity_target_key;
}

void AiWalk::walk_around() {
    target_type_ = WalkTargetType::Random;
    entity_target_key_ = default_target_key;
}
