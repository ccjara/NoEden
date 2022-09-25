#include "catalog.hxx"

Archetype* Catalog::create_archetype(std::string_view name) {
    auto it = archetypes_.find(std::string(name));
    if (it != archetypes_.end()) {
        Log::error("Could not create archetype: {} already exists", name);
        return nullptr;
    }
    auto archetype = std::make_unique<Archetype>();
    Archetype* raw_ptr = archetype.get();
    archetype->name = name;
    archetypes_[archetype->name] = std::move(archetype);
    Log::info("Archetype {} created", raw_ptr->name);
    return raw_ptr;
}

const Archetype* Catalog::archetype(std::string_view name) {
    auto iter = archetypes_.find(std::string(name));
    if (iter == archetypes_.end()) {
        return nullptr;
    }
    return iter->second.get();
}