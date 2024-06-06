#ifndef NOEDEN_GAME_PLAYER_CONTROLLER_HXX
#define NOEDEN_GAME_PLAYER_CONTROLLER_HXX

#include "world/player_controller.hxx"

class IEntityReader;
class IActionCreator;
class KeyDownEvent;

class GamePlayerController : public IPlayerController {
public:
    explicit GamePlayerController(
        IEntityReader* entity_reader,
        IActionCreator* action_creator,
        EventManager* events
    );
private:
    bool on_key_down(KeyDownEvent& e);

    /**
     * Attempts to schedule a move action for the player
     * @param direction Direction to move in
     * @return true if the action was successfully scheduled
     */
    bool move_relative(const Vec2<i32>& direction);

    EventManager* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    IActionCreator* action_creator_ = nullptr;
};

#endif