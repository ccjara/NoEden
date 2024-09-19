#ifndef NOEDEN_LIFE_COMPONENT_HXX
#define NOEDEN_LIFE_COMPONENT_HXX

#include "component/generic_component.hxx"

/**
 * @brief Classifies an entity as living or in any other way animated
 */
class Life : public GenericComponent<Life, ComponentType::Life> {
public:
    bool alive = true;
};

#endif
