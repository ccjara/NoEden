#pragma once

#include "resource/resource.hxx"
#include "resource/resource_traits.hxx"

class Catalog;
class ResourceIndex;
class ResourceRepository;
class Shader;

class ResourceManager {
public:
    explicit ResourceManager(ServiceLocator& svc);

    bool initialize();

    /**
     * @brief Returns a shader by its key or nullptr if not found.
     *
     * Loads the shader if not already loaded.
     */
    [[nodiscard]] Shader* shader(std::string_view key);

    /**
     * @brief Loads the catalog when necessary and returns it.
     */
    [[nodiscard]] Catalog* catalog();

    ~ResourceManager();
private:
    /**
     * @brief Common resource loading function
     */
    template <typename Target, ResourceLike Resource, typename Loader>
    Target* target(std::string_view key, std::unordered_map<std::string, std::unique_ptr<Target>>& container, Loader loader);

    /**
     * @brief Contains all loaded shaders indexed by key
     */
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;

    /**
     * @brief Loads and sets up a shader based on an indexed shader resource
     */
    std::unique_ptr<Shader> load_shader(const ShaderResource& shader_resource) const;

    /**
     * @brief Repository used to retrieve resource data from
     */
    std::unique_ptr<ResourceRepository> repository_ = nullptr;

    /**
     * @brief Index of resources
     */
    std::unique_ptr<ResourceIndex> index_ = nullptr;

    /**
    * @brief Current catalog instance
    */
    std::unique_ptr<Catalog> catalog_ = nullptr;

    /**
     * @brief Loads the json file from the resource repository and returns it as a json_value on success
     */
    std::optional<edenjson::json_value> json_from_path(std::string_view path) const;

    ServiceLocator& svc_;
    ThreadPool *tp_ = nullptr;
};
