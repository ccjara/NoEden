#ifndef NOEDEN_ACTION_QUEUE_HXX
#define NOEDEN_ACTION_QUEUE_HXX

#include "action/action_creator.hxx"
#include "action/action_processor.hxx"

struct WorldContext;

class ActionQueue : public IActionCreator, public IActionProcessor {
public:
    void initialize(WorldContext* world_context);

    /**
     * @copydoc IActionCreator::create_action
    */
    [[nodiscard]] CreateActionResult create_action(ActionType type, Entity& actor) override;

    /**
     * @copydoc IActionProcessor::process_actions
    */
    void process_actions() override;
private:
    static constexpr inline float min_speed = 0.001f;

    std::unique_ptr<Action> instantiate_action(ActionType type);

    /**
     * Calculates the energy required to perform the given action
     *
     * @param action Action to perform
     * @return Energy required to perform the action
     */
    static constexpr float calculate_cost(ActionType type, float actor_speed);

    /**
     * Returns the base cost of the given action type
     *
     * @param type Action type
     * @return Base cost of the action
     */
    static constexpr float base_cost(ActionType type);

    std::vector<std::unique_ptr<Action>> actions_;

    WorldContext* world_context_ = nullptr;
};

#endif
