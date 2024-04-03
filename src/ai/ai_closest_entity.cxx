#include "ai_closest_entity.hxx"

AiNodeState AiClosestEntity::visit(AiContext& context) {
    if (!context.entity) {
        return mod_state(AiNodeState::Failed);
    }
    const int radius = context.entity->vision_radius;

    // OPTIMIZE: quadtree
    for (const auto& entity : Scene::entities()) {
        if (entity->id == context.entity_id) {
            continue;
        }
        if (entity->position.manhattan_distance(context.entity->position) > radius) {
            continue;
        }
        context.blackboard.set(found_target_key, entity->id);
        return mod_state(AiNodeState::Succeeded);
    }

    // no unit in range matching the filter found
    return mod_state(AiNodeState::Failed);
}

void AiClosestEntity::clear() {
    mod_state(AiNodeState::Ready);
}

void AiClosestEntity::set_found_target_key(std::string&& key) {
    found_target_key = std::move(key);
}

void AiClosestEntity::set_found_target_key(const std::string& key) {
    found_target_key = key;
}
