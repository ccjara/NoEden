#include "scripts/api/catalog_api.hxx"

#include "ai/ai_closest_entity.hxx"
#include "ai/ai_condition.hxx"
#include "ai/ai_selector.hxx"
#include "ai/ai_walk.hxx"
#include "ai/condition_type.hxx"
#include "ai/condition_resolver.hxx"
#include "catalog/catalog.hxx"
#include "component/render.hxx"
#include "component/behavior.hxx"
#include "component/life.hxx"
#include "component/vision/vision.hxx"
#include "entity/archetype.hxx"
#include "resource/resource_manager.hxx"

class IActionCreator;
class IEntityReader;

bool CatalogApi::initialize() {
    catalog_ = svc_->get<ResourceManager>()->catalog();
    if (!catalog_) {
        LOG_ERROR("CatalogApi failed to initialize: failed to get Catalog");
        return false;
    }
    condition_resolver_ = svc_->get<ConditionResolver>();
    if (!condition_resolver_) {
        LOG_ERROR("CatalogApi failed to initialize: failed to get ConditionResolver");
        return false;
    }
    return true;
}

void CatalogApi::on_register(Script& script) {
    script.define_enum(
        "ComponentType",
        std::make_tuple("Behavior", ComponentType::Behavior),
        std::make_tuple("Render", ComponentType::Render),
        std::make_tuple("Skills", ComponentType::Skills),
        std::make_tuple("Vision", ComponentType::Vision),
        std::make_tuple("Life", ComponentType::Life)
    );

    script.define_enum(
        "AiNodeType",
        std::make_tuple("Sequence", AiNodeType::Sequence),
        std::make_tuple("Condition", AiNodeType::Condition),
        std::make_tuple("Selector", AiNodeType::Selector),
        std::make_tuple("ClosestEntity", AiNodeType::ClosestEntity),
        std::make_tuple("Walk", AiNodeType::Walk)
    );

    script.define_enum(
        "AiConditionType",
        std::make_tuple("AlwaysFalse", ConditionType::AlwaysFalse),
        std::make_tuple("AlwaysTrue", ConditionType::AlwaysTrue),
        std::make_tuple("IsAlive", ConditionType::IsAlive)
    );

    luabridge::getGlobalNamespace(script)
        .beginClass<CatalogApi>("Catalog")
            .addFunction("create_archetype", &CatalogApi::create_archetype)
            .addFunction("clear_archetypes", &CatalogApi::clear_archetypes)
        .endClass();

    expose(script, this, "catalog");
}

void CatalogApi::clear_archetypes() {
    catalog_->clear_archetypes();
}

void CatalogApi::create_archetype(const luabridge::LuaRef& ref) {
    if (!ref.isTable()) {
        LOG_ERROR("Expected archetype data to be a table");
        return;
    }
    if (!ref["name"].isString()) {
        LOG_ERROR("Archetype specification has no name");
        return;
    }
    auto archetype = catalog_->create_archetype(ref["name"].cast<const char*>().value());
    if (!archetype) {
        return;
    }
    const auto speed_ref = ref["speed"];
    if (speed_ref.isNumber()) {
        archetype->speed = speed_ref.cast<f32>().value();
    }
    const auto components_ref = ref["components"];
    if (components_ref.isTable()) {
        const auto length = components_ref.length();

        auto parse_components = [this, archetype](const luabridge::LuaRef& component_ref) -> void {
            if (component_ref.isNil()) {
                return;
            }
            const auto type_ref = component_ref["type"];
            if (!type_ref.isNumber()) {
                LOG_ERROR("Invalid {} component: type must be a string", archetype->name);
            }
            const auto component_type_unsafe = type_ref.cast<i32>().value();
            switch (static_cast<ComponentType>(component_type_unsafe)) {
                case ComponentType::Render: {
                    const auto glyph_ref = component_ref["glyph"];
                    auto component_ptr = new Render();
                    archetype->components.emplace_back(component_ptr);
                    auto& info = component_ptr->display_info();
                    if (!glyph_ref.isNumber()) {
                        LOG_WARN("Glyph in render component spec of {} is not a number", archetype->name);
                        return;
                    }
                    const auto color_ref = component_ref["color"];
                    info.glyph = glyph_ref.cast<i32>().value();
                    info.visible = true;

                    if (color_ref.isNumber()) {
                        info.color = Color(color_ref.cast<i32>().value());
                    }
                    return;
                }
                case ComponentType::Behavior: {
                    return add_behavior_component(*archetype, component_ref);
                }
                case ComponentType::Life: {
                    archetype->components.emplace_back(std::make_unique<Life>());
                    break;
                }
                case ComponentType::Vision: {
                    auto vision = std::make_unique<Vision>();

                    const auto vision_radius_ref = component_ref["radius"];
                    i32 radius = 1;
                    if (vision_radius_ref.isNumber()) {
                        radius = vision_radius_ref.cast<i32>().value();

                        if (radius <= 0) {
                            LOG_WARN("Invalid vision config in {}: vision radius must be greater than 0", archetype->name);
                            radius = 1;
                        }
                    } else {
                        LOG_WARN("Invalid vision config in {}: vision radius not set. Defaulting to 1", archetype->name);
                    }

                    vision->set_vision_radius(radius);
                    archetype->components.push_back(std::move(vision));
                    return;
                }
                default:
                    LOG_WARN("Invalid {} component: unknown type id {}", archetype->name, component_type_unsafe);
            }
        };

        for (i32 nz_offset = 1U; nz_offset <= length; ++nz_offset) {
            parse_components(components_ref[nz_offset]);
        }
    }
}

AiNodeType parse_node_type(const luabridge::LuaRef& ref) {
    if (!ref.isNumber()) {
        return AiNodeType::None;
    }

    switch (const auto unsafe_value = static_cast<AiNodeType>(ref.cast<i32>().value())) {
        case AiNodeType::None:
        case AiNodeType::Selector:
        case AiNodeType::Condition:
        case AiNodeType::Sequence:
        case AiNodeType::ClosestEntity:
        case AiNodeType::Walk:
            return unsafe_value;
        default:
            return AiNodeType::None;
    }
}

std::unique_ptr<AiNode> CatalogApi::create_behavior_node(const luabridge::LuaRef& ref) {
    const auto type_ref = ref["type"];
    const auto type = parse_node_type(type_ref);

    std::unique_ptr<AiNode> base_node_ptr;

    switch (type) {
        case AiNodeType::Selector: {
            base_node_ptr = std::make_unique<AiSelector>();
            auto node_ptr = static_cast<AiSelector*>(base_node_ptr.get());

            const auto children_ref = ref["children"];
            if (!children_ref.isTable()) {
                break;
            }

            const auto children_length = children_ref.length();

            // for_each please!
            i32 last_priority = 0;
            for (i32 i = 1; i <= children_length; ++i) {
                const auto priority_ref= ref["priority"];
                i32 priority;
                if (priority_ref.isNumber()) {
                    priority = priority_ref.cast<i32>().value();
                } else {
                    priority = last_priority + 1;
                }
                last_priority = priority;
                node_ptr->add(priority, create_behavior_node(children_ref[i]));
            }

            break;
        }
        case AiNodeType::Sequence: {
            base_node_ptr = std::make_unique<AiSelector>();
            auto node_ptr = static_cast<AiSelector*>(base_node_ptr.get());

            const auto children_ref = ref["children"];
            if (!children_ref.isTable()) {
                break;
            }

            const auto children_length = children_ref.length();

            // for_each please!
            i32 last_priority = 0;
            for (i32 i = 1; i <= children_length; ++i) {
                const auto priority_ref= ref["priority"];
                i32 priority;
                if (priority_ref.isNumber()) {
                    priority = priority_ref.cast<i32>().value();
                } else {
                    priority = last_priority + 1;
                }
                last_priority = priority;
                node_ptr->add(priority, create_behavior_node(children_ref[i]));
            }

            break;
        }
        case AiNodeType::ClosestEntity: {
            base_node_ptr = std::make_unique<AiClosestEntity>();
            auto node_ptr = static_cast<AiClosestEntity*>(base_node_ptr.get());
            const auto found_target_key_ref = ref["found_target_key"];
            if (found_target_key_ref.isString()) {
                node_ptr->set_found_target_key(found_target_key_ref.cast<std::string>().value());
            }
            break;
        }
        case AiNodeType::Walk: {
            base_node_ptr = std::make_unique<AiWalk>();
            auto node_ptr = static_cast<AiWalk*>(base_node_ptr.get());
            const auto walk_target_key = ref["walk_target_key"];
            if (walk_target_key.isString()) {
                node_ptr->target_entity(walk_target_key.cast<std::string>().value());
            } else {
                node_ptr->walk_around();
            }
            break;
        }
        case AiNodeType::Condition: {
            const auto condition_type_ref = ref["condition"];
            if (!condition_type_ref.isNumber()) {
                LOG_ERROR("Invalid condition node configuration: must have a `condition` number property identifying its condition type");
                return nullptr;
            }
            const auto condition_type_unsafe = condition_type_ref.cast<i32>().value();
            const auto condition_type = static_cast<ConditionType>(condition_type_unsafe);

            auto condition_fn = condition_resolver_->resolve_condition(condition_type);

            if (!condition_fn) {
                LOG_ERROR("Failed to resolve condition function for condition type {}", condition_type_unsafe);
                return nullptr;
            }

            const auto child_ref = ref["child"];
            if (!child_ref.isTable()) {
                LOG_ERROR("Invalid condition node configuration: must have a `child` table property, specifying the child node to visit if the condition is true");
                break;
            }
            auto child = create_behavior_node(child_ref);

            if (!child) {
                LOG_ERROR("Failed to create child node for condition node");
                return nullptr;
            }

            base_node_ptr = std::make_unique<AiCondition>(std::move(condition_fn), std::move(child));
            break;
        }
        default:
            LOG_ERROR("Unknown node type id {}", static_cast<i32>(type));
    }
    return base_node_ptr;
}


void CatalogApi::add_behavior_component(Archetype& archetype, const luabridge::LuaRef& ref) {
    const auto root_ref = ref["root"];
    if (!root_ref.isTable()) {
        LOG_ERROR("Root in behavior component spec of {} is not a table.", archetype.name);
        return;
    }
    auto behavior = new Behavior();
    archetype.components.emplace_back(behavior);
    behavior->set_root(create_behavior_node(root_ref));
    if (!behavior->root()) {
        LOG_WARN("Behavior tree constructed without any nodes in archetype {}", archetype.name);
    }
}
