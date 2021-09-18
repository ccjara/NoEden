#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../actor/actor.hxx"
#include "../grid.hxx"
#include "tile_builder.hxx"
#include "fov.hxx"

/**
 * @brief Contains and manages actors.
 */
class Scene {
    using ActorContainer = std::vector<std::unique_ptr<Actor>>;
public:
    Scene();

    /**
     * @brief Returns a pointer to an actor for the given id or nullptr if not found.
     */
    Actor* get_actor_by_id(u64 id) const;

    /**
     * @brief Constructs an actor of the given archetype and returns it.
     * 
     * The actor can be further configured after creation.
     */
    Actor& create_actor(const Archetype* archetype);

    /**
     * @brief Provides write access to the actor container.
     */
    ActorContainer& actors();

    /**
     * @brief Provides readonly access to the actor container.
     */
    const ActorContainer& read_actors() const;

    /**
     * @brief Provides write access to the tile container.
     */
    Grid<Tile>& tiles();

    /**
     * @brief Provides readonly access to the tile grid.
     */
    const Grid<Tile>& read_tiles() const;

    void update_fov(Actor* player);
private:
    ActorContainer actors_;
    std::unordered_map<u64, Actor*> actors_by_id_;

    Grid<Tile> tiles_;
};

// TODO: add some walls and implement shadow casting

#endif
