#ifndef NOEDEN_PLAYER_CONTROLLER_HXX
#define NOEDEN_PLAYER_CONTROLLER_HXX

#include "world/player_controller.hxx"

class IEntityReader;
class IActionCreator;

class PlayerController {
public:
    explicit PlayerController(
        IEntityReader* entity_reader,
        IActionCreator* action_creator,
        EventManager* events
    );
private:
    EventResult on_key_down(KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_;

    /**
     * Attempts to schedule a move action for the player
     * @param direction Direction to move in
     * @return true if the action was successfully scheduled
     */
    bool move_relative(const WorldPos& direction);

    EventManager* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    IActionCreator* action_creator_ = nullptr;

};

#endif
