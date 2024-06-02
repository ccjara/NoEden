#ifndef JARALYN_ACTION_CREATOR_HXX
#define JARALYN_ACTION_CREATOR_HXX

class Entity;
class Action;
enum class ActionType;

// defined further below
enum class CreateActionError;
struct CreateActionResult;


class IActionCreator {
public:
    /**
     * @brief Factory method to create an action for the given entity.
     * 
     * Injects all necessary dependencies into the action and sets the actor.
     * The action's cost is also calculated here and depends on the speed of the
     * entity. Later changes to the Entity's speed will will not affect this
     * cost.
     * 
     * Creation of the action may fail due to various reasons in which case 
     * the result will contain an error and no action.
     * 
     * @param type Type of the action to create.
     * @param actor Entity that will perform the action.
     * 
     * @return CreateActionResult Result of the creation.
     */
    [[nodiscard]] virtual CreateActionResult create_action(ActionType type, Entity& actor) = 0;

    virtual ~IActionCreator() = default;
};

enum class CreateActionError {
    /**
     * @brief Action was created successfully.
     */
    None,

    /**
     * @brief Actor speed is too low
     */
    ActorSpeedTooLow,

    /**
     * @brief Action type does not have to any implementation
     */
    InvalidType,
};

struct CreateActionResult {
    /**
     * @brief Error that occurred during the creation of the action.
     */
    CreateActionError error;

    /**
     * @brief Created action or null if error is set
     */
    Action* action;

    /**
     * @brief Checks if the creation of the action failed.
     */
    bool failed() const {
        return error != CreateActionError::None;
    }
};

#endif
