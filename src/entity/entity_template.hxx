#pragma once

#include "component/component.hxx"

/**
 * \brief Template used as a base when creating new Entities of a certain kind
 */
struct EntityTemplate {
    /**
     * \brief Unique ID
     */
    std::string id = "";

    /**
     * \brief Catalog source file name
     */
    std::string source = {};

    /**
    * \brief Parent entity template id
    */
    std::string extends = {};

    /**
     * \brief Pointer to parent entity template
     */
    const EntityTemplate* parent = nullptr;

    /**
     * \brief Base speed every Entity of this kind inherits.
     */
    f32 speed = 0;

    /**
     * \brief Components which will be created for each Entity of this entity_template
     *
     * TODO: needs unique existence checking on *some* component types
     */
    std::vector<std::unique_ptr<Component>> components;

    /**
     * \brief Copies inheritable properties from the given parent into this entity template
     */
    void inherit_from(const EntityTemplate& parent) {
        components.clear();
        for (const auto& component : parent.components) {
            components.push_back(component->clone());
        }
        this->parent = &parent;
        this->speed = parent.speed;
    }
};
