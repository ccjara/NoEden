#pragma once

#include "catalog/material.hxx"
#include "entity/entity_template.hxx"

class Catalog {
public:
    /**
     * @brief Creates and takes ownership of an empty EntityTemplate with the given id.
     *
     * You may modify the returned EntityTemplate further using the returned pointer.
     * Returns nullptr if an EntityTemplate with the given id already exists.
     */
    EntityTemplate* create_entity_template(std::string_view id);

    /**
     * @brief Returns an EntityTemplate by name or nullptr if the EntityTemplate does not exist.
     */
    const EntityTemplate* entity_template(std::string_view name) const;

    /**
     * @brief Removes all entity_templates from the catalog
     */
    void clear_entity_templates();

    /**
     * @brief Replaces the current material catalog with the given one
     */
    void set_materials(std::unordered_map<std::string, std::unique_ptr<Material>>&& materials);

    /**
     * \brief Replaces the current entity template catalog with the given one
     */
    void set_entity_templates(std::unordered_map<std::string, std::unique_ptr<EntityTemplate>>&& templates);

    /**
     * @brief Returns all materials having the given category
     */
    std::vector<Material*> materials_by_category(std::string_view category) const;

    /**
     * @brief Returns a material by its id or nullptr if not found
     */
    Material* material(std::string_view id) const;

    ~Catalog();
protected:
    std::unordered_map<std::string, std::unique_ptr<EntityTemplate>> entity_templates_ = {};

    std::unordered_map<std::string, std::unique_ptr<Material>> materials_ = {};
};
