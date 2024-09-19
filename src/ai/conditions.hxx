#ifndef NOEDEN_CONDITIONS_HXX
#define NOEDEN_CONDITIONS_HXX

#include "ai/ai_context.hxx"
#include "ai/condition_resolver.hxx"
#include "entity/entity.hxx"
#include "component/life.hxx"

inline void register_conditions(ConditionResolver& resolver) {
    ConditionRegistry conditions {
        {
            ConditionType::AlwaysFalse,
            std::make_shared<ConditionFn>([](AiContext&) { return false; })
        },
        {
            ConditionType::AlwaysTrue,
            std::make_shared<ConditionFn>([](AiContext&) { return true; })
        },
        {
            ConditionType::IsAlive,
            std::make_shared<ConditionFn>([](const AiContext& context) {
                assert(context.entity);

                if (const Life* life_component = context.entity->component<Life>(); life_component) {
                    return life_component->alive;
                }

                return false;
            })
        },
    };

    resolver.register_conditions(std::move(conditions));
}

#endif
