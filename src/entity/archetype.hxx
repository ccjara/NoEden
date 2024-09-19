#ifndef NOEDEN_ARCHETYPE_HXX
#define NOEDEN_ARCHETYPE_HXX

#include "component/component.hxx"

/**
 * @brief Represents a template to create Entities of one 'kind'
 */
struct Archetype {
    /**
     * @brief Name defined in lua which can later be used for identification.
     */
    std::string name = "";

    /**
     * @brief Base speed every Entity of this kind inherits.
     */
    f32 speed = 0;

    /**
     * @brief Components which will be created for each Entity of this archetype
     *
     * TODO: needs unique existence checking on *some* component types
     */
    std::vector<std::unique_ptr<Component>> components;
};

#endif
