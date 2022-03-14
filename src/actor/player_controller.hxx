#ifndef JARALYN_PLAYER_CONTROLLER_HXX
#define JARALYN_PLAYER_CONTROLLER_HXX

#include "actor.hxx"
#include "action_queue.hxx"
#include "move_action.hxx"
#include "../input/input_state.hxx"
#include "../input/input_event.hxx"

/**
 * @brief Reacts to input and generates player actions based on the input.
 * 
 * This class only deals with Actors, i.e. units on the scene. Input dealing 
 * with UI is handled elsewhere.
 */
class PlayerController {
public:
    explicit PlayerController(ActionQueue& queue, EventManager& events);

    /**
     * @brief Declares the given actor as the new player to control.
     * 
     * May be and is initially null to signify no player currently exists.
     * 
     * The player controller can only control one player at a time.
     */
    void control(Actor* actor);

    /**
     * @brief Returns the player action for this tick (if any).
     * 
     * May be null if the user did not request any action for this frame.
     */
    Action* pull_player_action();

    /**
     * @brief Returns the current actor regarded as a player if any.
     */
    Actor* player();
private:

    bool on_key_press(KeyDownEvent& e);

    Actor* player_;
    Action* player_action_ { nullptr };
    ActionQueue& queue_;
    EventManager& events_;
};

#endif
