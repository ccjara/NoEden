#include "entity/entity_manager.hxx"
#include "entity/archetype.hxx"
#include "entity/entity.hxx"
#include "entity/entity_event.hxx"
#include "world/world_context.hxx"

void EntityManager::initialize(WorldContext* world_context) {
    assert(world_context);
    world_context_ = world_context;
}

Entity* EntityManager::entity(Id id) {
    auto it = index_by_id_.find(id);
    if (it == index_by_id_.end()) {
        return nullptr;
    }
    if (it->second >= entities_.size()) {
        return nullptr;
    }
    return entities_[it->second].get();
}

const Entity* EntityManager::entity(Id id) const {
    const auto it = index_by_id_.find(id);
    if (it == index_by_id_.cend()) {
        return nullptr;
    }
    if (it->second >= entities_.size()) {
        return nullptr;
    }
    return entities_[it->second].get();
}

EntityManager::EntityContainer& EntityManager::entities() {
    return entities_;
}

const EntityManager::EntityContainer& EntityManager::entities() const {
    return entities_;
}

Entity& EntityManager::create_entity(const Archetype& archetype, const WorldPos& position) {
    {
        auto entity = std::make_unique<Entity>();

        // copy common properties from archetype into entity
        entity->speed = archetype.speed;
        entity->name = archetype.name;

        for (const auto& archetype_component : archetype.components) {
            entity->add_component(archetype_component->clone());
        }

        entity->position = position;
        entities_.push_back(std::move(entity));
    }

    Entity* entity = entities_.back().get();
    index_by_id_[entity->id] = entities_.size() - 1;

    world_context_->events->trigger<EntityCreatedEvent>(entity);

    return *entity;
}

Entity* EntityManager::player() {
    return controlled_entity_;
}

const Entity* EntityManager::player() const {
    return controlled_entity_;
}

ControlEntityResult EntityManager::set_controlled_entity(Id id) {
    if (id == null_id) {
        return set_controlled_entity(nullptr);
    }

    Entity* next_player = entity(id);
    if (next_player == nullptr) {
        return ControlEntityResult::EntityNotFound;
    }
    return set_controlled_entity(next_player);
}

ControlEntityResult EntityManager::set_controlled_entity(Entity* entity) {
    Entity* current_player = player();
    if (current_player) {
        current_player->on_player_detached();
    }

    if (entity != nullptr) {
        entity->energy = 0;
        entity->on_player_attached();
    }
    controlled_entity_ = entity;
    LOG_DEBUG("Entity {} is now controlled", entity ? entity->id : 0);

    return ControlEntityResult::Success;
}
