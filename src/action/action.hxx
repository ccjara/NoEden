#ifndef NOEDEN_ACTION_HXX
#define NOEDEN_ACTION_HXX

#include "action/action_result.hxx"
#include "action/action_type.hxx"

class Entity;
struct WorldContext;

/**
 * @brief Base class for an action performed by entities.
 */
class Action {
public:
    /**
     * @brief Extension point for subclasses defining a concrete action.
     *
     * Returns true if the action was performed successfully.
     */
    virtual ActionResult perform() = 0;

    /**
     * @brief Extension point for subclasses to initialize the action.
     */
    virtual void initialize();

    /**
     * @brief Returns the type of this action.
     */
    ActionType type() const;

    /**
     * @brief Returns the entity performing this action.
     */
    Entity* entity() const;

    /**
     * @brief Returns the cost of this action.
     */
    float cost() const;

    /**
     * @brief Returns the speed of the entity performing this action.
     */
    float speed() const;

    virtual ~Action() = default;

protected:
    friend class ActionQueue;

    explicit Action(ActionType type);

    ActionType type_ = ActionType::None;

    /**
     * @brief Points to the Entity performing this action.
     */
    Entity* entity_ = nullptr;

    /**
     * @brief World context this action operates in
     */
    WorldContext* world_context_ = nullptr;

    /**
     * @brief Speed at which this action performs.
     *
     * This speed is unaffected by changes to the entity's speed being made
     * after this action was created.
     */
    float speed_ = 0;

    /**
     * @brief Calculated cost during action creation
     */
    float cost_ = 0;
};

#endif
