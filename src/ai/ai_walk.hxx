#ifndef JARALYN_AI_WALK_HXX
#define JARALYN_AI_WALK_HXX

#include "generic_ai_node.hxx"
#include "../entity/entity.hxx"
#include "../entity/move_action.hxx"
#include "../scene/scene.hxx"

enum class WalkTargetType {
    Random,
    Entity,
};

class AiWalk : public GenericAiNode<AiWalk> {
public:
    void clear() override;

    AiNodeState visit(AiContext& context) override;

    /**
     * @brief Perform walk actions which target an entity located by the id stored at the given key in the blackbox
     */
    void target_entity(const std::string& entity_target_key);

    /**
     * @brief Perform a walk action in a random direction
     */
    void walk_around();
private:
    static constexpr const char* default_target_key = "walk_target";

    inline AiNodeState perform_walk_to_entity(AiContext& context);
    inline AiNodeState perform_walk_around(AiContext& context);

    /**
     * @brief What kind of movement target to use
     */
    WalkTargetType target_type_ = WalkTargetType::Random;

    /**
     * @brief The target key, if applicable to use when targeting an entity
     */
    std::string entity_target_key_ = default_target_key;
};

#endif
