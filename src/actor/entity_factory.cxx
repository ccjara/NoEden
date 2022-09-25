#include "entity_factory.hxx"

std::unique_ptr<Actor> EntityFactory::create(const Archetype& archetype) {
    auto actor = std::make_unique<Actor>();

    for (const auto& archetype_component : archetype.components) {
        actor->components_.push_back(archetype_component->clone());
    }
    actor->reindex_components();

    return actor;
}
