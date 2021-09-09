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
     * @brief Constructs an actor of the given archetype and returns it.
     * 
     * The actor can be further configured after creation.
     */
    Actor& create_actor(const Archetype* archetype) {
        auto& actor { actors_.emplace_back(new Actor(archetype)) };

        return *actor.get();
    }
private:
    ActorContainer actors_;
};

#endif
