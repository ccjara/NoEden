#ifndef NOEDEN_RESOURCE_TRAIT_HXX
#define NOEDEN_RESOURCE_TRAIT_HXX

#include "resource/resource_type.hxx"

template<typename T>
struct ResourceTraits;

class ShaderResource;

template<>
struct ResourceTraits<ShaderResource> {
    static constexpr ResourceType type = ResourceType::Shader;
};

#endif
