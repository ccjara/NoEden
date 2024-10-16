#include "catalog/catalog.hxx"
#include "entity/entity_template.hxx"

Catalog::~Catalog() {
}

EntityTemplate* Catalog::create_entity_template(std::string_view id) {
    auto it = entity_templates_.find(std::string(id));
    if (it != entity_templates_.end()) {
        LOG_ERROR("Could not create entity template {}: already exists", id);
        return nullptr;
    }
    auto entity_template = std::make_unique<EntityTemplate>();
    EntityTemplate* raw_ptr = entity_template.get();
    entity_template->id = id;
    entity_templates_[entity_template->id] = std::move(entity_template);
    LOG_INFO("Entity template {} created", raw_ptr->id);
    return raw_ptr;
}

const EntityTemplate* Catalog::entity_template(std::string_view name) const {
    auto iter = entity_templates_.find(std::string(name));
    if (iter == entity_templates_.end()) {
        return nullptr;
    }
    return iter->second.get();
}

void Catalog::clear_entity_templates() {
    entity_templates_.clear();
    LOG_INFO("Entity templates cleared");
}

void Catalog::set_materials(std::unordered_map<std::string, std::unique_ptr<Material>>&& materials) {
    materials_ = std::move(materials);
}

void Catalog::set_entity_templates(std::unordered_map<std::string, std::unique_ptr<EntityTemplate>>&& templates) {
    entity_templates_ = std::move(templates);
}

Material* Catalog::material(std::string_view id) const {
    const auto iter = materials_.find(std::string(id));
    if (iter == materials_.end()) {
        return nullptr;
    }
    return iter->second.get();
}

std::vector<Material*> Catalog::materials_by_category(std::string_view category) const {
    // optimize
    std::vector<Material*> result;
    const std::string cat = std::string(category);
    for (const auto& [_, material] : materials_) {
        if (material->categories.contains(cat)) {
            result.push_back(material.get());
        }
    }
    return result;
}
