#include "action/action_queue.hxx"
#include "action/action.hxx"
#include "action/action_event.hxx"
#include "action/move_action.hxx"
#include "action/null_action.hxx"
#include "entity/entity.hxx"
#include "world/world_context.hxx"

void ActionQueue::initialize(WorldContext* world_context) {
    assert(world_context);
    world_context_ = world_context;
}

CreateActionResult ActionQueue::create_action(ActionType type, Entity& actor) {
    if (type == ActionType::None) {
        return { CreateActionError::InvalidType, nullptr };
    }
    if (actor.speed <= 0) {
        return { CreateActionError::ActorSpeedTooLow, nullptr };
    }
    const float cost = calculate_cost(type, actor.speed);

    std::unique_ptr<Action> action = instantiate_action(type);
    action->entity_ = &actor;
    action->speed_ = actor.speed;
    action->cost_ = cost;

    action->world_context_ = world_context_;
    actor.energy_reserved += cost;

    action->initialize();

    Action* action_raw_ptr = actions_.emplace_back(std::move(action)).get();

    return { CreateActionError::None, action_raw_ptr };
}

std::unique_ptr<Action> ActionQueue::instantiate_action(ActionType type) {
    switch (type) {
        case ActionType::None:
            return std::make_unique<NullAction>();
        case ActionType::Move:
            return std::make_unique<MoveAction>();
        default:
            return {};
    }
}

void ActionQueue::process_actions() {
    Profiler::timer("ActionQueue").start();
    std::sort(
        actions_.begin(),
        actions_.end(),
        [](const auto& a, const auto& b) {
            return a->speed_ > b->speed_;
        }
    );
    for (auto& action: actions_) {
        assert(action);
        Entity* entity = action->entity_;
        if (!entity) {
            continue;
        }
        const auto speed = action->entity_->speed;
        if (speed <= 0) {
            continue;
        }
        // recalculate cost in case the actor speed changed after the action was created
        const float energy_needed = calculate_cost(action->type_, speed);
        if (energy_needed > entity->energy) {
            continue;
        }
        action->perform();
        entity->energy -= energy_needed;
    }
    actions_.clear();
    Profiler::timer("ActionQueue").stop();

    world_context_->events->trigger<ActionQueueProcessed>();
}

constexpr float ActionQueue::calculate_cost(ActionType type, float actor_speed) {
    if (actor_speed <= 0) [[unlikely]] {
        actor_speed = min_speed;
    }
    return base_cost(type) / actor_speed;
}

constexpr float ActionQueue::base_cost(ActionType type) {
    // TODO: scriptable / configurable later
    switch (type) {
        case ActionType::None:
        default:
            return 0;
        case ActionType::Move:
            return 100;
    }
}
