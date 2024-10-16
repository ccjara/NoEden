#include "ai/ai_node.hxx"

AiNodeState AiNode::mod_state(AiNodeState s) {
    state_ = s;
    return s;
}

AiNodeState AiNode::state() const {
    return state_;
}

bool AiNode::failed() const {
    return state_ == AiNodeState::Failed;
}

void AiNode::initialize(AiContext&) {
}

static const std::unordered_map<std::string_view, AiNodeType> ai_node_type_map {
    {"selector", AiNodeType::Selector},
    {"sequence", AiNodeType::Sequence},
    {"closest_entity", AiNodeType::ClosestEntity},
    {"walk", AiNodeType::Walk},
    {"condition", AiNodeType::Condition},
};

AiNodeType parse_node_type(std::string_view type) {
    if (const auto it = ai_node_type_map.find(type); it != ai_node_type_map.cend()) {
        return it->second;
    }
    return AiNodeType::Invalid;
}
