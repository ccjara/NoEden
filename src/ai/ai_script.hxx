#ifndef NOEDEN_AI_SCRIPT_HXX
#define NOEDEN_AI_SCRIPT_HXX

#include "ai/generic_ai_node.hxx"

class AiScript : public GenericAiNode<AiScript> {
public:
    void clear() override;

    AiNodeState visit(AiContext& context) override;
};

#endif
