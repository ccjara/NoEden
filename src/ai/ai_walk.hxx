#ifndef JARALYN_AI_WALK_HXX
#define JARALYN_AI_WALK_HXX

#include "generic_ai_node.hxx"
#include "../entity/entity.hxx"
#include "../entity/move_action.hxx"
#include "../scene/scene.hxx"

class AiWalk : public GenericAiNode<AiWalk> {
public:
    void clear() override;

    AiNodeState visit(AiContext& context) override;
};

#endif
