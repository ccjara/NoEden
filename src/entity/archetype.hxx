#ifndef JARALYN_ARCHETYPE_HXX
#define JARALYN_ARCHETYPE_HXX

#include "components/component.hxx"

/**
 * @brief Represents a template to create Entities of one 'kind'
 */
struct Archetype {
    /**
     * @brief Name defined in lua which can later be used for identification.
     */
    std::string name;

    /**
     * @brief Base speed every Entity of this kind inherits.
     */
    i32 speed;

    /**
     * @brief Components which will be created for each Entity of this archetype
     */
    std::vector<std::unique_ptr<Component>> components;
};

#endif
