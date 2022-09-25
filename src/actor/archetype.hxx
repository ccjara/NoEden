#ifndef JARALYN_ARCHETYPE_HXX
#define JARALYN_ARCHETYPE_HXX

#include "components/component.hxx"

/**
 * @brief Actor templates used to construct Actors of one kind.
 */
struct Archetype {
    /**
     * @brief Name defined in lua which can later be used for identification.
     */
    std::string name;

    /**
     * @brief Base speed every Actor of this kind inherits.
     */
    i32 speed;

    /**
     * @brief Components which will be created for each Actor of this archetype
     */
    std::vector<std::unique_ptr<Component>> components;
};

#endif
