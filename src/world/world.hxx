#ifndef JARALYN_WORLD_HXX
#define JARALYN_WORLD_HXX

class IEntityReader;
class IPlayerController;
class IActionProcessor;
class Entity;
struct PlayerActionCommitted;

class World {
public:
    explicit World(IEntityReader* entity_reader,IActionProcessor* action_processor,EventManager* events);

    /**
     * @brief Sets the player controller for this world
     * 
     * @param controller Player controller to bind
     */
    void bind_player_controller(IPlayerController* controller);
private:
    bool on_player_action_committed(const PlayerActionCommitted& e);

    /**
     * @brief Player controller bound to this world
     */
    IPlayerController* player_controller = nullptr;

    EventManager* events_ = nullptr;
    IEntityReader* entity_reader_ = nullptr;
    IActionProcessor* action_processor_ = nullptr;
};

#endif
