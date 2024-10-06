#pragma once

#include "resource/resource_type.hxx"

template<typename T>
struct ResourceTraits;

class CatalogResource;
class ShaderResource;

template<>
struct ResourceTraits<CatalogResource> {
    static constexpr ResourceType type = ResourceType::Catalog;
};

template<>
struct ResourceTraits<ShaderResource> {
    static constexpr ResourceType type = ResourceType::Shader;
};
