#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../actor/actor.hxx"
#include "../actor/move_action.hxx"
#include "../grid.hxx"
#include "tile_builder.hxx"
#include "../input/input_event.hxx"
#include "fov.hxx"

/**
 * @brief Contains and manages actors.
 */
class Scene {
    using ActionContainer = std::vector<std::unique_ptr<Action>>;
    using ActorContainer = std::vector<std::unique_ptr<Actor>>;
public:
    static void init();
    static void shutdown();

    /**
     * @brief Returns a pointer to an actor for the given id or nullptr if not found.
     */
    static Actor* get_actor_by_id(u64 id);

    /**
     * @brief Constructs an actor of the given archetype and returns it.
     *
     * The actor can be further configured after creation.
     */
    static Actor& create_actor(const Archetype* archetype);

    /**
     * @brief Provides write access to the actor container.
     */
    static ActorContainer& actors();

    /**
     * @brief Provides readonly access to the actor container.
     */
    static const ActorContainer& read_actors();

    /**
     * @brief Provides write access to the tile container.
     */
    static Grid<Tile>& tiles();

    /**
     * @brief Provides readonly access to the tile grid.
     */
    static const Grid<Tile>& read_tiles();

    /**
     * @brief Writes the field of view to the display
     */
    static void update_fov();

    /**
     * @brief Executes each action in order of the Actions' calculated speed.
     *
     * The queue is sorted before it is processed, faster actions will get
     * processed first. Speed is calculated upon pushing an action onto the
     * queue and is unaffected by speed changes of other actions.
     */
    static void perform_actions();

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
    static Action& create_action(Actor* actor, ActorArgs&&... args) {
        assert(actor);

        auto& action = *actions_.emplace_back(
            new A(std::forward<ActorArgs>(args)...)
        ).get();

        action.actor = actor;
        action.cost = action.speed * action.base_cost();
        return action;
    }

    /**
     * @brief Declares the given actor as the new player to control.
     *
     * May be and is initially null to signify no player currently exists.
     *
     * The player controller can only control one player at a time.
     */
    static void set_player(Actor* actor);

    /**
     * @brief Returns the current actor regarded as a player if any.
     */
    static Actor* player();
private:
    static bool on_key_down(KeyDownEvent& e);

    static inline ActorContainer actors_;
    static inline ActionContainer actions_;
    static inline Actor* player_ = nullptr;
    static inline Action* player_action_ = nullptr;
    static inline std::unordered_map<u64, Actor*> actors_by_id_;

    static inline Grid<Tile> tiles_;
};

#endif
