#pragma once

#include "resource/resource.hxx"

class CatalogResource : public Resource {
public:
    constexpr static const char* compound_id = "catalog";

    explicit CatalogResource(std::string_view path) : Resource(ResourceType::Catalog, compound_id, path) {
    }
};
