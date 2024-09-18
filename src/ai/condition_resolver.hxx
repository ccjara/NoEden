#ifndef NOEDEN_AI_CONDITION_RESOLVER_HXX
#define NOEDEN_AI_CONDITION_RESOLVER_HXX

#include "ai/condition_type.hxx"

struct AiContext;

using ConditionFn = std::function<bool(AiContext&)>;
using ConditionRegistry = std::unordered_map<ConditionType, std::shared_ptr<ConditionFn>>;

/**
 * @brief Resolves condition functions used by AI nodes
 */
class ConditionResolver {
public:
    /**
     * @brief Takes ownership of the given registry replaces the current registry
     */
    void register_conditions(ConditionRegistry&& conditions);

    /**
     * @brief Takes ownership of the condition function and associates it with the condition type
     * @param type Condition type to associate with the condition function
     * @param condition Condition function
     */
    void register_condition(ConditionType type, ConditionFn&& condition);

    /**
     * @brief Resolves a condition function by the given type
     * @param type Condition type identifying the condition
     * @return Shared pointer to the condition function or nullptr if not found
     */
    std::shared_ptr<ConditionFn> resolve_condition(ConditionType type);
private:
    /**
     * @brief Registry of condition functions
     */
    ConditionRegistry conditions_ {};

    /**
     * @brief Default condition function to return when a condition is not found
     */
     ConditionFn default_condition_ = [] (AiContext&) { return false; };
};

#endif
