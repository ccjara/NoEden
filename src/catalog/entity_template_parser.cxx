#include "catalog/behavior_parser.hxx"
#include "catalog/entity_template_parser.hxx"
#include "component/behavior.hxx"
#include "component/life.hxx"
#include "component/render.hxx"
#include "component/vision/vision.hxx"
#include "framework/topological_sort.hxx"

void read_components(
    ParseEntityTemplatesInput input,
    EntityTemplate& entity_template,
    const edenjson::json_value& root
);
void read_component_render(EntityTemplate& entity_template, const edenjson::json_value& component_root);
void read_component_behavior(
    ParseEntityTemplatesInput input,
    EntityTemplate& entity_template,
    const edenjson::json_value& component_root
);
void read_component_life(EntityTemplate& entity_template, const edenjson::json_value& component_root);
void read_component_vision(EntityTemplate& entity_template, const edenjson::json_value& component_root);

std::unordered_map<std::string, std::unique_ptr<EntityTemplate>> parse_entity_templates(
ParseEntityTemplatesInput input
) {
    std::unordered_map<std::string, std::unique_ptr<EntityTemplate>> templates;
    std::unordered_map<std::string, std::vector<std::string>> dependencies;

    // construct dependency graph
    for (const auto& [file, root] : input.template_files) {
        auto entity_template = std::make_unique<EntityTemplate>();

        entity_template->source = file;

        const auto& spec = root["entity_template"];

        if (!spec.is_object()) {
            LOG_ERROR("Entity template object not found in entity template catalog file {}", file);
            continue;
        }

        entity_template->id = spec["id"].as_string().value_or("");
        if (entity_template->id.empty()) {
            LOG_ERROR("ID of entity template catalog file {} must be a non-empty string", file);
            continue;
        }

        auto& dependants = dependencies[entity_template->id];

        entity_template->extends = spec["extends"].as_string().value_or("");

        if (!entity_template->extends.empty()) {
            dependants.push_back(entity_template->id);
        }

        templates[entity_template->id] = std::move(entity_template);
    }

    const auto sort_result = topological_sort(dependencies);

    if (sort_result.status == TopologicalSortStatus::CycleDetected) {
        LOG_ERROR("Cycle detected in entity template catalog");
        return {};
    }

    // process in topological order
    for (const auto& id : sort_result.sorted_entities) {
        if (!templates.contains(id)) {
            LOG_ERROR("Integrity error: entity template {} not found in preliminary entity template catalog", id);
            return {};
        }
        auto& entity_template = templates[id];

        const auto root_iter = input.template_files.find(entity_template->source);
        if (root_iter == input.template_files.end()) {
            LOG_ERROR("Integrity error: source table of entity template {} not found", id);
            return {};
        }
        const auto root = root_iter->second;
        const auto template_spec = root["entity_template"];

        assert(template_spec.is_object()); // validated in the first pass

        // inherit properties from parent entity_template
        if (!entity_template->extends.empty()) {
            if (!templates.contains(entity_template->extends)) {
                LOG_ERROR("Integrity error: parent entity_template {} not found", entity_template->extends);
                return {};
            }
            entity_template->inherit_from(*templates[entity_template->extends]);
        }

        entity_template->speed = static_cast<f32> (template_spec["speed"].as_number().value_or(0));

        read_components(input, *entity_template, template_spec);
    }

    return templates;
}

void read_components(
    ParseEntityTemplatesInput input,
    EntityTemplate& entity_template,
    const edenjson::json_value& root
) {
    for (const auto& [key, component_spec] : root["components"].each_property()) {
        switch (parse_component_type(key)) {
            case ComponentType::Render:
                read_component_render(entity_template, component_spec);
                break;
            case ComponentType::Behavior:
                read_component_behavior(input, entity_template, component_spec);
                break;
            case ComponentType::Life:
                read_component_life(entity_template, component_spec);
                break;
            case ComponentType::Vision:
                read_component_vision(entity_template, component_spec);
                break;
            default:
                LOG_ERROR("Unknown component type {} in entity template {}", key, entity_template.id);
        }
    }
}

void read_component_render(
    EntityTemplate& entity_template,
    const edenjson::json_value& component_root
) {
    const auto glyph = static_cast<i32> (component_root["glyph"].as_number().value_or(-1));
    const auto color_value = static_cast<i32> (component_root["color"].as_number().value_or(0xFFFFFF));

    if (glyph == -1) {
        LOG_ERROR("Glyph in render component spec of {} must be a number >= 0", entity_template.id);
        return;
    }

    auto render = std::make_unique<Render>();
    auto& display_info = render->display_info();

    display_info.glyph = glyph;
    display_info.color = Color(color_value);
    display_info.visible = true;

    entity_template.components.emplace_back(std::move(render));
}

void read_component_life(EntityTemplate& entity_template, const edenjson::json_value& component_root) {
    entity_template.components.emplace_back(std::make_unique<Life>());
}

void read_component_vision(EntityTemplate& entity_template, const edenjson::json_value& component_root) {
    const auto radius = static_cast<i32> (component_root["radius"].as_number().value_or(1));

    auto vision = std::make_unique<Vision>();

    vision->set_vision_radius(radius);
    entity_template.components.emplace_back(std::move(vision));
}

void read_component_behavior(
    ParseEntityTemplatesInput input,
    EntityTemplate& entity_template,
    const edenjson::json_value& component_root
) {
    const auto& behavior_root = component_root["root"];

    if (!behavior_root.is_object()) {
        LOG_ERROR("Root in behavior component spec of {} must be an object", entity_template.id);
        return;
    }

    auto root_node = parse_ai_node(input.condition_resolver, behavior_root);

    if (root_node == nullptr) {
        LOG_ERROR("Failed to read behavior tree from behavior component spec of {}", entity_template.id);
        return;
    }

    auto behavior = std::make_unique<Behavior>();
    behavior->set_root(std::move(root_node));
    entity_template.components.emplace_back(std::move(behavior));
}
