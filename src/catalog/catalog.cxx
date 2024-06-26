#include "catalog/catalog.hxx"
#include "entity/archetype.hxx"

Archetype* Catalog::create_archetype(std::string_view name) {
    auto it = archetypes_.find(std::string(name));
    if (it != archetypes_.end()) {
        LOG_ERROR("Could not create archetype: {} already exists", name);
        return nullptr;
    }
    auto archetype = std::make_unique<Archetype>();
    Archetype* raw_ptr = archetype.get();
    archetype->name = name;
    archetypes_[archetype->name] = std::move(archetype);
    LOG_INFO("Archetype {} created", raw_ptr->name);
    return raw_ptr;
}

const Archetype* Catalog::archetype(std::string_view name) const {
    auto iter = archetypes_.find(std::string(name));
    if (iter == archetypes_.end()) {
        return nullptr;
    }
    return iter->second.get();
}

void Catalog::clear_archetypes() {
    archetypes_.clear();
    LOG_INFO("Archetypes cleared");
}
