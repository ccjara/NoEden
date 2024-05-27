#include "ai_walk.hxx"
#include "../entity/move_action.hxx"
#include "../scene/scene.hxx"

void AiWalk::clear() {
    mod_state(AiNodeState::Ready);
    target_type_ = WalkTargetType::Random;
    entity_target_key_ = default_target_key;
}

AiNodeState AiWalk::visit(AiContext& context) {
    static int dir = 0;
    auto entity = Scene::get_entity_by_id(context.entity_id);
    if (!entity) {
        return mod_state(AiNodeState::Failed);
    }

    if (entity->energy < MoveAction::BASE_COST / entity->speed) {
        return mod_state(AiNodeState::Failed);
    }

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
    auto target = Scene::get_entity_by_id(*target_id);
    if (!target) {
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

    Scene::create_action<MoveAction>(context.entity, position);

    return mod_state(AiNodeState::Failed); // TODO: this must be changed later when repeatable actions (by conditions) are implemented
}

AiNodeState AiWalk::perform_walk_around(AiContext& context) {
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

    Scene::create_action<MoveAction>(context.entity, pos);
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