#include "entity_factory.hxx"

std::unique_ptr<Entity> EntityFactory::create(const Archetype& archetype) {
    auto entity = std::make_unique<Entity>();

    for (const auto& archetype_component : archetype.components) {
        auto component = archetype_component->clone();
        // TODO: add method on entity to do this so the friend class EntityFactory on Component can be removed
        component->entity_id_ = entity->id;
        entity->components_.push_back(std::move(component));
    }
    entity->reindex_components();

    return entity;
}
