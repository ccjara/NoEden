#include "entity_factory.hxx"

std::unique_ptr<Entity> EntityFactory::create(const Archetype& archetype) {
    auto entity = std::make_unique<Entity>();

    // copy common properties from archetype into entity
    entity->speed = archetype.speed;
    entity->vision_radius = archetype.vision_radius;
    entity->name = archetype.name;

    for (const auto& archetype_component : archetype.components) {
        auto component = archetype_component->clone();
        // TODO: add method on entity to do this so the friend class EntityFactory on Component can be removed
        component->set_owner(entity.get());
        entity->components_.push_back(std::move(component));
    }
    entity->reindex_components();

    return entity;
}
