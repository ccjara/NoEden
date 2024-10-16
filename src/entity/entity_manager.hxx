#ifndef NOEDEN_ENTITY_MANAGER_HXX
#define NOEDEN_ENTITY_MANAGER_HXX

struct WorldContext;
class Entity;
struct EntityTemplate;

enum class ControlEntityResult {
    Success,
    EntityAlreadyControlled,
    EntityNotFound,
    EntityNotControllable,
};

class EntityManager {
public:
    using EntityContainer = std::vector<std::unique_ptr<Entity>>;

    void initialize(WorldContext* world_context);

    /**
     * @brief Returns an entity by id or nullptr if not found
     */
    Entity* entity(Id id);

    /**
     * @brief Returns an entity by id or nullptr if not found
     */
    const Entity* entity(Id id) const;

    /**
     * @brief Provides write access to the entity container
     */
    EntityContainer& entities();

    /**
     * @brief Provides read access to the entity container
     */
    const EntityContainer& entities() const;

    /**
     * @brief Returns the player entity or nullptr if not found
     */
    Entity* player();

    /**
     * @brief Returns the player entity or nullptr if not found
     */
    const Entity* player() const;

    /**
     * @brief Creates an Entity of the given entity_template and returns it.
     *
     * The Entity can be further configured after creation.
     */
    Entity& create_entity(const EntityTemplate& entity_template, const WorldPos& position);

    /**
     * @brief Sets the player controlled entity
     */
    ControlEntityResult set_controlled_entity(Entity* entity);

    /**
     * @brief Sets the player controlled entity.
     *
     * Resolves the entity and then calls set_controlled_entity(Entity* entity)
     */
    ControlEntityResult set_controlled_entity(Id id);

private:
    /**
     * @brief Primary (owning) container of all entities
     */
    EntityContainer entities_;

    /**
     * @brief Indexes entities by their ID.
     *
     * The value (index) points to the entity's position in the entities_ vector.
     */
    std::unordered_map<Id, size_t> index_by_id_ = {};

    /**
     * @brief Entity currently controlled by the player
     */
    Entity* controlled_entity_ = nullptr;

    WorldContext* world_context_ = nullptr;
};

#endif
