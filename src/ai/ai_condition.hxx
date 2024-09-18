#ifndef NOEDEN_AI_CONDITION_HXX
#define NOEDEN_AI_CONDITION_HXX

#include "ai/generic_ai_node.hxx"

/**
 * @brief Condition decorator node which runs a bool function to determine its state
 */
class AiCondition : public GenericAiNode<AiCondition> {
    using ConditionFnPtr = std::shared_ptr<std::function<bool(AiContext&)>>;
public:
    explicit AiCondition(ConditionFnPtr condition, std::unique_ptr<AiNode>&& child);

    AiNodeState visit(AiContext& context) override;

    void clear() override;


    AiCondition() = default;
    AiCondition(const AiCondition& other);
    AiCondition(AiCondition&& other) noexcept;
    AiCondition& operator=(AiCondition other);

    friend void swap(AiCondition& lhs, AiCondition& rhs) noexcept {
        using std::swap;
        swap(lhs.child_, rhs.child_);
        swap(lhs.condition_, rhs.condition_);
    }
protected:
    /**
     * @brief Condition function to run
     */
    ConditionFnPtr condition_ = nullptr;

    /**
     * @brief Child node to visit if the condition is true
     */
    std::unique_ptr<AiNode> child_ = nullptr;
};

#endif
