#pragma once

#include <ai/ai_node.hxx>

class ConditionResolver;

/**
 * \brief Parses an AI node from the given JSON object.
 *
 * \returns The parsed AI node, or nullptr if parsing failed.
 */
std::unique_ptr<AiNode> parse_ai_node(ConditionResolver& condition_resolver, const edenjson::json_value& value);
