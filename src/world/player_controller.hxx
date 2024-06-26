#ifndef NOEDEN_PLAYER_CONTROLLER_HXX
#define NOEDEN_PLAYER_CONTROLLER_HXX

#include "world/player_controller.hxx"

struct WorldContext;

class PlayerController {
public:
    void initialize(WorldContext* world_context);
private:
    EventResult on_key_down(KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_;

    /**
     * Attempts to schedule a move action for the player
     * @param direction Direction to move in
     * @return true if the action was successfully scheduled
     */
    bool move_relative(const WorldPos& direction);

    WorldContext* world_context_ = nullptr;
};

#endif
