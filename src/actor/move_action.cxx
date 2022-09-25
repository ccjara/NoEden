#include "move_action.hxx"

MoveAction::MoveAction(Vec2<i32> destination) :
    destination { destination } {
}

u32 MoveAction::base_cost() const {
    return 100;
}

bool MoveAction::perform() {
    if (!actor) {
        return false;
    }
    actor->energy -= base_cost();

    if (destination.x < 0 || destination.y < 0) {
        return false;
    }
    const auto& dest_tile = Scene::tiles().at(destination);
    if (dest_tile->solid) {
        return false;
    }

    if (dest_tile->type == TileType::Water) {
        Skills* skills = actor->component<Skills>();
        if (skills) {
            skills->increase_progress(SkillId::Swim, 1);
        }
    }

    actor->position = destination;

    return true;
}
