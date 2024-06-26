#ifndef NOEDEN_AI_CLOSEST_ENTITY_HXX
#define NOEDEN_AI_CLOSEST_ENTITY_HXX

#include "ai/generic_ai_node.hxx"

class EntityManager;

/**
 * @brief Scans the area in range and finds the closest entity if any
 */
class AiClosestEntity : public GenericAiNode<AiClosestEntity> {
public:
    AiNodeState visit(AiContext& context) override;

    /**
     * @brief Sets (move) the blackboardkey to use when storing the closest entity found
     */
    void set_found_target_key(std::string&& key);

    /**
     * @brief Sets (copy) the blackboard key to use when storing the closest entity found
     */
    void set_found_target_key(const std::string& key);

    void clear() override;
private:
    /**
     * @brief Blackboard key to use when storing the closest entity found
     */
    std::string found_target_key = "closest_entity";
};

#endif
