#ifndef NOEDEN_ENTITY_FACTORY_HXX
#define NOEDEN_ENTITY_FACTORY_HXX

class Entity;
struct Archetype;

class EntityFactory {
public:
    /**
     * @brief Creates an entity based on the given Archetype template
     * 
     * @param archetype Archetype template to create the entity from
     * @return std::unique_ptr<Entity> Created entity
     */
    [[nodiscard]] static std::unique_ptr<Entity> create(const Archetype& archetype);
};

#endif
