#pragma once

#include "catalog/material.hxx"
#include "framework/topological_sort.hxx"

class MaterialReader {
public:
    std::unordered_map<std::string, std::unique_ptr<Material>> read_materials() {
        std::unordered_map<std::string, std::unique_ptr<Material>> materials;
        std::unordered_map<std::string, std::vector<std::string>> dependencies;

        // construct dependency graph
        for (const auto& [file, table] : tables_) {
            auto material = std::make_unique<Material>();

            material->source = file;

            const auto& material_spec = table["material"];

            if (!material_spec.is_object()) {
                LOG_ERROR("Material object not found in material catalog file {}", file);
                continue;
            }

            material->id = material_spec["id"].as_string().value_or("");
            if (material->id.empty()) {
                LOG_ERROR("ID of material catalog file {} must be a non-empty string", file);
                continue;
            }

            material->extends = material_spec["extends"].as_string().value_or("");

            if (!material->extends.empty()) {
                dependencies[material->extends].push_back(material->id);
            }

            materials[material->id] = std::move(material);
        }

        const auto sort_result = topological_sort(dependencies);

        if (sort_result.status == TopologicalSortStatus::CycleDetected) {
            LOG_ERROR("Cycle detected in material catalog");
            return {};
        }

        // process materials in topological order
        for (const auto& material_id : sort_result.sorted_entities) {
            if (!materials.contains(material_id)) {
                LOG_ERROR("Integrity error: material {} not found in preliminary material catalog", material_id);
                return {};
            }
            auto& material = materials[material_id];

            if (!tables_.contains(material->source)) {
                LOG_ERROR("Integrity error: source table of material {} not found", material_id);
                return {};
            }

            const auto root = tables_[material->source];
            const auto material_spec = root["material"];

            assert(material_spec.is_object()); // validated in the first pass

            // inherit properties from parent material
            if (!material->extends.empty()) {
                if (!materials.contains(material->extends)) {
                    LOG_ERROR("Integrity error: parent material {} not found", material->extends);
                    return {};
                }
                material->inherit_from(*materials[material->extends]);
            }

            // categories
            for (const auto& element : material_spec["categories"].as_array().value_or(edenjson::json_array{})) {
                const auto& category = element.as_string().value_or("");
                if (category.empty()) {
                    LOG_WARN("Material category in material catalog file {} must be a non-empty string", material_id);
                    continue;
                }

                if (std::ranges::find(material->categories, category) != material->categories.end()) {
                    continue;
                }

                material->categories.emplace(category);
            }

            // thermal properties
            const auto& thermal_properties = material_spec["thermal"];

            if (thermal_properties.is_object()) {
                material->thermal_properties.ignites_at = thermal_properties["ignites_at"].as_number().value_or(0);
            }
        }

        return materials;
    }

    void add(const std::string& file, edenjson::json_value&& root) {
        tables_[file] = std::move(root);
    }

private:
    std::unordered_map<std::string, edenjson::json_value> tables_;
};
