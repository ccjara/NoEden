#include "ai/condition_type.hxx"

static const std::unordered_map<std::string_view, ConditionType> condition_type_map {
    {"always_false", ConditionType::AlwaysFalse},
    {"always_true", ConditionType::AlwaysTrue},
    {"is_alive", ConditionType::IsAlive},
};

ConditionType parse_condition_type(std::string_view value) {
    if (const auto it = condition_type_map.find(value); it != condition_type_map.cend()) {
        return it->second;
    }
    return ConditionType::Invalid;
}
