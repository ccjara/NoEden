#ifndef NOEDEN_AI_CONTEXT_HXX
#define NOEDEN_AI_CONTEXT_HXX

#include "ai/blackboard.hxx"

class Entity;
struct WorldContext;

struct AiContext {
    /**
     * @brief Id of the entity this Ai assumes control of
     */
    Id entity_id = null_id;

    /**
     * @brief Entity this Ai assumes control of
     */
    Entity* entity = nullptr;

    /**
     * @brief Globally accessible blackboard (from the perspective of any node in the behavior tree)
     */
    Blackboard blackboard;

    /**
     * @brief World context
     */
    WorldContext* world_context = nullptr;
};

#endif
