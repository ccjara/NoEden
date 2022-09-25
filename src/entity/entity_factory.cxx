#include "entity_factory.hxx"

std::unique_ptr<Entity> EntityFactory::create(const Archetype& archetype) {
    auto entity = std::make_unique<Entity>();

    for (const auto& archetype_component : archetype.components) {
        entity->components_.push_back(archetype_component->clone());
    }
    entity->reindex_components();

    return entity;
}
