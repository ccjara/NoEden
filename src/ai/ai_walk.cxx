#include "ai_walk.hxx"

void AiWalk::clear() {
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

    auto pos = entity->position;

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

    Scene::create_action<MoveAction>(entity, pos);
    return mod_state(AiNodeState::Succeeded);
}
