#include "ai/condition_resolver.hxx"

std::shared_ptr<ConditionFn> ConditionResolver::resolve_condition(ConditionType type) {
    if (const auto it = conditions_.find(type); it != conditions_.cend()) {
        return it->second;
    }
    return nullptr;
}

void ConditionResolver::register_conditions(ConditionRegistry&& conditions) {
    conditions_ = std::move(conditions);
}

void ConditionResolver::register_condition(ConditionType type, ConditionFn&& condition) {
    conditions_[type] = std::make_shared<ConditionFn>(std::move(condition));
}
