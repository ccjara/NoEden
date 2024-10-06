#ifndef NOEDEN_CATALOG_HXX
#define NOEDEN_CATALOG_HXX

#include "catalog/material.hxx"
#include "entity/archetype.hxx"

class Catalog {
public:
    /**
     * @brief Creates and takes ownership of an empty Archetype with the given name.
     *
     * You may modify the returned Archetype further using the returned pointer.
     * Returns nullptr if an Archetype with the given name already exists.
     */
    Archetype* create_archetype(std::string_view name);

    /**
     * @brief Returns an Archetype by name or nullptr if the Archetype does not exist.
     */
    const Archetype* archetype(std::string_view name) const;

    /**
     * @brief Removes all archetypes from the catalog
     */
    void clear_archetypes();

    /**
     * @brief Replaces the current material catalog with the given one
     */
    void set_materials(std::unordered_map<std::string, std::unique_ptr<Material>>&& materials);

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
    std::unordered_map<std::string, std::unique_ptr<Archetype>> archetypes_ = {};

    std::unordered_map<std::string, std::unique_ptr<Material>> materials_ = {};
};

#endif
