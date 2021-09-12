#ifndef JARALYN_SCENE_HXX
#define JARALYN_SCENE_HXX

#include "../actor/actor.hxx"

/**
 * @brief Contains and manages actors.
 */
class Scene {
    using ActorContainer = std::vector<std::unique_ptr<Actor>>;
public:
    /**
     * @brief Provides write access to the actor container.
     */
    ActorContainer& actors() {
        return actors_;
    }

    /**
     * @brief Provides readonly access to the actor container.
     */
    const ActorContainer& read_actors() const {
        return actors_;
    }

    /**
     * @brief Returns a pointer to an actor for the given id or nullptr if not found.
     */
    Actor* get_actor_by_id(u64 id) const {
        auto it { actors_by_id_.find(id) };
        return it == actors_by_id_.end() ? nullptr : it->second;
    }

    /**
     * @brief Constructs an actor of the given archetype and returns it.
     * 
     * The actor can be further configured after creation.
     */
    Actor& create_actor(const Archetype* archetype) {
        auto& actor { actors_.emplace_back(new Actor(archetype)) };
        actors_by_id_[actor->id] = actor.get();
        return *actor.get();
    }
private:
    ActorContainer actors_;

    std::unordered_map<u64, Actor*> actors_by_id_;
};

#endif
