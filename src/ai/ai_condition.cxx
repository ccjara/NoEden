#include "ai/ai_condition.hxx"

AiCondition& AiCondition::operator=(AiCondition other) {
    swap(*this, other);
    return *this;
}

AiCondition::AiCondition(const AiCondition& other) {
    child_ = other.child_->clone();
    condition_ = other.condition_;
}

AiCondition::AiCondition(AiCondition&& other) noexcept : AiCondition() {
    swap(*this, other);
}

AiCondition::AiCondition(ConditionFnPtr condition, std::unique_ptr<AiNode>&& child):
    condition_(std::move(condition)),
    child_(std::move(child)) {
}

AiNodeState AiCondition::visit(AiContext& context) {
    if (condition_ && (*condition_)(context) && child_) {
        return child_->visit(context);
    }
    return mod_state(AiNodeState::Failed);
}

void AiCondition::clear() {
    mod_state(AiNodeState::Ready);
}
