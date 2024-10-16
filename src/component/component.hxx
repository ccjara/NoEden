#pragma once

enum class ComponentType {
    Invalid = 0,
    Skills = 1,
    Behavior = 2,
    Render = 3,
    Vision = 4,
    Life = 5,
};

class Entity;

/**
 * @brief Component interface.
 *
 * Derive from GenericComponent instead when creating new Component classes.
 */
class Component {
public:
    virtual ~Component() = default;

    /**
     * @brief Returns the type of this component
     */
    ComponentType type() const;

    /**
     * @brief Heap-allocates a new component instance and returns it as a unique_ptr.
     *
     * You are required to manage ownership past the return.
     */
    [[nodiscard]] virtual std::unique_ptr<Component> clone() const = 0;

    /**
     * @brief Implementation defined update routine of this component
     */
    virtual void update();

    /**
     * @brief Sets the entity this component belongs to
     */
    void set_owner(Entity* entity);

    /**
     * @brief Called after all actions have been performed in the current turn
     */
    virtual void on_after_actions();

    /**
     * @brief Called when the player assumes control of the owning entity
     */
    virtual void on_player_attached();

    /**
     * @brief Called when the player relinquishes control of the owning entity
     */
    virtual void on_player_detached();
protected:
    friend class EntityManager;

    /**
     * @brief Derive components from GenericComponent instead.
     */
    explicit Component(ComponentType type);

    /**
     * @brief Runtime type of this component used to downcast Component pointers
     */
    ComponentType type_ = ComponentType::Invalid;

    /**
     * @brief Entity this component belongs to.
     *
     * Set by the entity manager when constructing the entity and its components from its entity_template.
     */
    Entity* entity_ = nullptr;

    /**
     * @brief Called when the owning entity of this component is updated
     */
    virtual void on_owner_updated();
};

template<typename T>
concept ComponentDerived = std::derived_from<T, Component>;

/**
* \brief Parses a component type from a string.
*
* \return A component type != `Invalid` on success, `Invalid` otherwise.
*/
[[nodiscard]] ComponentType parse_component_type(std::string_view value);
