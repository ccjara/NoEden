#include "ai/ai_closest_entity.hxx"
#include "ai/ai_condition.hxx"
#include "ai/ai_selector.hxx"
#include "ai/ai_sequence.hxx"
#include "ai/ai_walk.hxx"
#include "ai/condition_resolver.hxx"
#include "ai/condition_type.hxx"
#include "catalog/behavior_parser.hxx"

/**
 * \brief Parses an AI node from the given JSON object.
 *
 * Depending on the type of the node, this function may also be called recursively to parse child nodes.
 *
 * \returns The parsed AI node, or nullptr if parsing failed.
 */
std::unique_ptr<AiNode> parse_ai_node(ConditionResolver& condition_resolver, const edenjson::json_value& value) {
    const auto type_str = value["type"].as_string().value_or("");

    if (type_str.empty()) {
        LOG_ERROR("Invalid behavior node configuration: must have a non-empty `type` string property identifying its type");
        return nullptr;
    }

    switch (parse_node_type(type_str)) {
    case AiNodeType::Selector: {
        const auto& children = value["children"];
        if (!children.is_array()) {
            LOG_ERROR("Invalid selector node configuration: must have a `children` array property, specifying the child nodes to visit");
            return nullptr;
        }

        auto node = std::make_unique<AiSelector>();

        i32 last_priority = 0;
        for (const auto& child : children.each_element()) {
            const auto priority = static_cast<i32>(value["priority"].as_number().value_or(last_priority + 1));
            last_priority = priority;
            if (auto child_node = parse_ai_node(condition_resolver, child)) {
                node->add(priority, std::move(child_node));
            }
        }

        return node;
    }
    case AiNodeType::Sequence: {
        const auto& children = value["children"];
        if (!children.is_array()) {
            LOG_ERROR("Invalid sequence node configuration: must have a `children` array property, specifying the child nodes to visit");
            return nullptr;
        }

        auto node = std::make_unique<AiSequence>();

        for (const auto& child : children.each_element()) {
            const auto priority = static_cast<i32>(value["priority"].as_number().value_or(0));
            if (auto child_node = parse_ai_node(condition_resolver, child)) {
                node->add(priority, std::move(child_node));
            }
        }

        return node;
    }
    case AiNodeType::ClosestEntity: {
        auto node = std::make_unique<AiClosestEntity>();
        const auto& found_target_key = value["found_target_key"].as_string().value_or("");
        if (!found_target_key.empty()) {
            node->set_found_target_key(found_target_key);
        }
        return node;
    }
    case AiNodeType::Walk: {
        auto node = std::make_unique<AiWalk>();
        const auto& walk_target_key = value["walk_target_key"].as_string().value_or("");
        if (!walk_target_key.empty()) {
            node->target_entity(walk_target_key);
        } else {
            node->walk_around();
        }
        return node;
    }
    case AiNodeType::Condition: {
        const auto& condition_type_str = value["condition"].as_string().value_or("");
        if (condition_type_str.empty()) {
            LOG_ERROR("Invalid condition node configuration: must have a non-empty `condition` string property identifying its condition type");
            return nullptr;
        }

        const auto condition_type = parse_condition_type(condition_type_str);

        if (condition_type == ConditionType::Invalid) {
            LOG_ERROR("Invalid condition type {}", condition_type_str);
            return nullptr;
        }

        auto condition_fn = condition_resolver.resolve_condition(condition_type);

        if (!condition_fn) {
            LOG_ERROR("Failed to resolve condition function for condition type {}", condition_type_str);
            return nullptr;
        }

        const auto& child = value["child"];
        if (!child.is_object()) {
            LOG_ERROR("Invalid condition node configuration: must have a `child` object property, specifying the child node to visit if the condition is true");
            break;
        }
        auto child_node = parse_ai_node(condition_resolver, child);

        if (!child_node) {
            LOG_ERROR("Failed to create child node for condition node");
            return nullptr;
        }

        return std::make_unique<AiCondition>(std::move(condition_fn), std::move(child_node));
    }
    default:
        LOG_ERROR("Unknown behavior node type {}", type_str);
    }
    return nullptr;
}
