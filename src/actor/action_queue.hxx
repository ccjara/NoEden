#ifndef JARALYN_ACTION_QUEUE_HXX
#define JARALYN_ACTION_QUEUE_HXX

#include "action.hxx"
#include "actor.hxx"

/**
 * @brief Stores and processes actions created by the AI.
 */
class ActionQueue {
    using ActionPtr = std::unique_ptr<Action>;
    using ActionContainer = std::vector<ActionPtr>;
public:
    /**
     * @brief Constructs an action in place inside the action queue.
     * 
     * The action's cost is calculated here and depends on the speed of the
     * actor. Later changes to the actor's speed will will not affect this
     * cost.
     * 
     * Returns a reference to the created action.
     */
    template<typename A, typename... ActorArgs>
    Action& push(Actor* actor, ActorArgs&&... args) {
        assert(actor);

        auto& action = *actions_.emplace_back(
            new A(std::forward<ActorArgs>(args)...)
        ).get();

        action.actor = actor;
        action.cost = action.speed * action.base_cost();
        return action;
    }

    /**
     * @brief Executes each action in order of the Action's calculated speed.
     * 
     * The queue is sorted before it is processed, faster actions will get
     * processed first. Speed is calculated upon pushing an action onto the 
     * queue and is unaffected by speed changes of other actions.
     */
    void process();
private:
    /**
     * @brief Sorts all actions currently in the action container by their speed.
     */
    void sort_by_speed();

    ActionContainer actions_;
};

#endif
