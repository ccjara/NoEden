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

            const auto material_table = table["material"].as_table();

            if (!material_table) {
                LOG_ERROR("Material table not found in material catalog file {}", file);
                continue;
            }

            material->id = (*material_table)["id"].value_or("");
            if (material->id.empty()) {
                LOG_ERROR("ID of material catalog file {} must be a non-empty string", file);
                continue;
            }

            material->extends = (*material_table)["extends"].value_or("");

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

            const auto root_table = tables_[material->source];
            const auto material_table = root_table["material"].as_table();

            // inherit properties from parent material
            if (!material->extends.empty()) {
                if (!materials.contains(material->extends)) {
                    LOG_ERROR("Integrity error: parent material {} not found", material->extends);
                    return {};
                }
                material->inherit_from(*materials[material->extends]);
            }

            // categories
            if (const auto categories_array = (*material_table)["categories"].as_array()) {
                i32 index = 0;
                for (const auto& element : *categories_array) {
                    auto category = element.value_or(std::string(""));
                    if (category.empty()) {
                        LOG_ERROR(
                            "Material category at index {} in material catalog file {} must be a non-empty string",
                            index,
                            material_id
                        );
                        continue;
                    }

                    if (std::ranges::find(material->categories, category) != material->categories.end()) {
                        continue;
                    }

                    ++index;
                    material->categories.emplace(std::move(category));
                }
            }

            // thermal properties
            if (const auto thermal_properties_table = (*material_table)["thermal"].as_table()) {
                material->thermal_properties.ignites_at = (*thermal_properties_table)["ignites_at"].value_or(0);
            }
        }

        return materials;
    }

    void add(const std::string& file, toml::table&& table) {
        tables_[file] = table;
    }

private:
    std::unordered_map<std::string, toml::table> tables_;
};
