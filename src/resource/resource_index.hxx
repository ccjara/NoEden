#pragma once

#include "resource/resource.hxx"
#include "resource/resource_traits.hxx"

class ResourceIndex {
public:
    /**
    * @brief Loads the resource index from the given stream
    */
    bool load(std::istream& stream);

    /**
     * @brief Returns a resource by its key or nullptr if not found or indexed under a different type
     */
    [[nodiscard]] Resource* resource(std::string_view key, ResourceType type);

    /**
     * @brief Returns a resource by its key or nullptr if not found or indexed under a different type
     */
    template<ResourceLike TResource>
    [[nodiscard]] TResource* resource(std::string_view key) {
        return static_cast<TResource*>(resource(key, ResourceTraits<TResource>::type));
    }

    ~ResourceIndex();
private:
    /**
    * @brief Indexes resources by key
    */
    std::unordered_map<std::string, std::unique_ptr<Resource>> resources_;

    bool index_catalog(const toml::table& table);
    bool index_shaders(const toml::table& table);
    bool index_shader(std::string_view key, const toml::array& stage_array);
};
