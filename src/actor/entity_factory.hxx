#ifndef JARALYN_ENTITY_FACTORY_HXX
#define JARALYN_ENTITY_FACTORY_HXX

#include "actor.hxx"
#include "archetype.hxx"

class EntityFactory {
public:
    static std::unique_ptr<Actor> create(const Archetype& archetype);
};

#endif
