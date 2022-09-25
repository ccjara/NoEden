#ifndef JARALYN_COMPONENT_HXX
#define JARALYN_COMPONENT_HXX

enum class ComponentType {
    Unknown = 0,
    Skills = 1,
    Behavior = 2,
    Render = 3,
};

class Component {
    friend class Actor;
public:
    explicit Component(ComponentType type);

    virtual ~Component() = default;

    [[nodiscard]] ComponentType type() const;

    static constinit const ComponentType static_type = ComponentType::Unknown;

    Id entity_id() const;

    virtual std::unique_ptr<Component> clone() const = 0;
protected:
    ComponentType type_ = ComponentType::Unknown;

    Id entity_id_ = null_id;

};

template<typename T>
concept ComponentDerived = std::derived_from<T, Component>;

#endif
