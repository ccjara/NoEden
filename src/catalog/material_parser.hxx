#pragma once

#include "catalog/material.hxx"

/**
 * \brief Parses the material catalog from the given JSON object
 *
 * \returns A map of materials, indexed by their IDs
 */
std::unordered_map<std::string, std::unique_ptr<Material>> parse_materials(
    const std::unordered_map<std::string, edenjson::json_value>& material_files
);
