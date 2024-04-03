#ifndef JARALYN_ACTOR_HXX
#define JARALYN_ACTOR_HXX

#include "components/component.hxx"

struct Entity {
    friend class EntityFactory;

    Entity();

    Id id;
    static inline Id next_id_ = 1U;

    std::string name = "NYI"; // TODO

    Vec2<i32> position;

    i32 vision_radius { 0 };
    i32 speed { 0 };
    i32 energy { 0 };

    /**
     * @brief Locates a component by the given template argument.
     *
     * Returns nullptr if the no component of that type currently exists.
     */
    template<ComponentDerived Comp>
    Comp* component() const {
        static_assert(Comp::static_type() != ComponentType::Unknown);
        auto iter = components_by_type_.find(Comp::static_type());
        if (iter == components_by_type_.end()) {
            return nullptr;
        }
        return static_cast<Comp*>(iter->second);
    }

    /**
     * @brief Constructs a component of the given type.
     *
     * If a component of that type already exists, this method will return nullptr and
     * will not create or insert the requested component.
     */
    template<ComponentDerived Comp, typename... ComponentArgs>
    Comp* add_component(ComponentArgs&&... args) {
        static_assert(Comp::static_type != ComponentType::Unknown);
        if (components_by_type_.find(Comp::static_type) != components_by_type_.end()) {
            return nullptr;
        }
        auto component = new Comp(std::forward<ComponentArgs>(args)...);
        component->entity_id_ = id;
        components_.emplace_back(component);
        reindex_components();
        return component;
    }

    void update(u64 dt);
private:
    std::vector<std::unique_ptr<Component>> components_;
    std::unordered_map<ComponentType, Component*> components_by_type_;

    /**
     * @brief Rebuilds dictionaries and caches of the component store.
     *
     * Should be called every time the component store changes in structure which
     * might cause pointers in the dictionaries to become invalidated.
     */
    void reindex_components();
};

#endif
