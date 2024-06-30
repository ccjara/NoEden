#include "ai/ai_closest_entity.hxx"
#include "component/vision/vision.hxx"
#include "entity/entity.hxx"
#include "entity/entity_manager.hxx"
#include "realm/realm.hxx"

void AiClosestEntity::initialize(AiContext& context) {
    entity_manager_ = context.entity->realm->services().get<EntityManager>();
    assert(entity_manager_);
}

AiNodeState AiClosestEntity::visit(AiContext& context) {
    if (!context.entity) {
        return mod_state(AiNodeState::Failed);
    }
    auto* entity_manager = context.entity->realm->services().get<EntityManager>();

    // OPTIMIZE: could subscribe to a `VisionRadiusUpdated` event to then
    //           cache the radius in the blackboard

    const auto* vision = context.entity->component<Vision>();
    if (vision == nullptr) {
        return mod_state(AiNodeState::Failed);
    }
    const auto radius = vision->vision_radius();

    // OPTIMIZE: quadtree
    for (const auto& entity : entity_manager->entities()) {
        if (entity->id == context.entity_id) {
            continue;
        }
        if (entity->position.manhattan_distance_to(context.entity->position) > radius) {
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
