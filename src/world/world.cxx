#include "world.hxx"
#include "entity/entity.hxx"
#include "entity/entity_reader.hxx"
#include "action/action_event.hxx"
#include "action/action.hxx"
#include "tile/tile_reader.hxx"
#include "tile/tile.hxx"
#include "action/action_processor.hxx"

World::World(
    IEntityReader* entity_reader,
    ITileReader* tile_reader,
    IActionProcessor* action_processor,
    EventManager* events
) : entity_reader_(entity_reader),
    events_(events),
    tile_reader_(tile_reader),
    action_processor_(action_processor) {
    assert(entity_reader_);
    assert(events_);
    assert(tile_reader_);
    assert(action_processor_);

    events_->on<PlayerActionCommitted>(this, &World::on_player_action_committed);
}

void World::bind_player_controller(IPlayerController* controller) {
    player_controller = controller;
}

bool World::on_player_action_committed(const PlayerActionCommitted& e) {
    if (e.action == nullptr) [[unlikely]] {
        return false;
    }

    // reset fov
    tile_reader_->tiles().each([](Tile& tile) { tile.fov = false; });

    // give all entities in the world energy according to the cost of the action
    const auto cost = e.action->cost();
    for (auto& entity: entity_reader_->entities()) {
        entity->update(cost);
    }

    // process actions in order
    action_processor_->process_actions();

    // inform entities that actions have been processed
    for (auto& entity: entity_reader_->entities()) {
        entity->on_after_actions();
    }

    return false;
}
