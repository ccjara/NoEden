#ifndef NOEDEN_RESOURCE_INDEX_HXX
#define NOEDEN_RESOURCE_INDEX_HXX

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

    void index_shaders(const toml::table& table);
    void index_shader(std::string_view key, const toml::array& stage_array);
};

#endif
