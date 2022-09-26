#ifndef JARALYN_AI_CONTEXT_HXX
#define JARALYN_AI_CONTEXT_HXX

struct AiContext {
    /**
     * @brief Id of the entity this Ai assumes control of
     */
    Id entity_id;

    explicit AiContext(Id entity_id);
};

#endif
