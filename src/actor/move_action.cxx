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

    const Vec2<u32> dest_upos = destination; // TODO: let's just use ints for simplicity's sake...
    const auto& dest_tile = scene->read_tiles().at(dest_upos);
    if (dest_tile->solid) {
        return false;
    }

    if (dest_tile->type == TileType::Water) {
        auto& swim_skill = actor->skills[SkillId::Swim];
        ++swim_skill.progress;
    }

    actor->position = destination;

    return true;
};
