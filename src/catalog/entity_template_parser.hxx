#pragma once

#include "entity/entity_template.hxx"

class ConditionResolver;

struct ParseEntityTemplatesInput {
    /**
    * \brief AI condition resolver reference
    */
    ConditionResolver& condition_resolver;

    /**
    * \brief Map of entity template data, indexed by their file names
    */
    const std::unordered_map<std::string, edenjson::json_value>& template_files;
};

/**
 * \brief Parses all entity templates from the given collection of JSON objects
 *
 * \param input Input data
 *
 * \returns A map of entity templates, indexed by their IDs
 */
[[nodiscard]] std::unordered_map<std::string, std::unique_ptr<EntityTemplate>> parse_entity_templates(
    ParseEntityTemplatesInput input
);
