#ifndef JARALYN_ENTITY_FACTORY_HXX
#define JARALYN_ENTITY_FACTORY_HXX

#include "entity.hxx"
#include "archetype.hxx"

class EntityFactory {
public:
    static std::unique_ptr<Entity> create(const Archetype& archetype);
};

#endif
