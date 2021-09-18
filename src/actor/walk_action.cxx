#include "walk_action.hxx"

WalkAction::WalkAction(Vec2<i32> destination) :
    destination { destination } {
}

u32 WalkAction::base_cost() const {
    return 100;
}

bool WalkAction::perform() {
    if (!actor) {
        return false;
    }
    actor->energy -= base_cost();

    if (destination.x < 0 || destination.y < 0) {
        return false;
    }

    const Vec2<u32> tile_dest = destination;
    if (scene->read_tiles().at(tile_dest)->solid) {
        return false;
    }

    actor->position = destination;

    return true;
};
