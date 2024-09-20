#include "gfx/shader.hxx"
#include "resource/resource_index.hxx"
#include "resource/resource_manager.hxx"
#include "resource/plain_resource_repository.hxx"
#include "resource/shader_resource.hxx"

ResourceManager::ResourceManager() :
    repository_(std::make_unique<PlainResourceRepository>()),
    index_(std::make_unique<ResourceIndex>()) {
    assert(repository_);
    assert(index_);
}

ResourceManager::~ResourceManager() {

}

bool ResourceManager::initialize() {
    constexpr auto INDEX_TOML_PATH = "resources/index.toml";

    auto file = std::ifstream(INDEX_TOML_PATH);
    if (!file) {
        LOG_ERROR("Could not load resource index from path {}: could not open file for reading", INDEX_TOML_PATH);
        return false;
    }

    if (!index_->load(file)) {
        LOG_ERROR("Could not load resource index");
        return false;
    }

    return true;
}

template <typename Target, ResourceLike Resource, typename Loader>
Target* ResourceManager::target(std::string_view key, std::unordered_map<std::string, std::unique_ptr<Target>>& container, Loader loader) {
    if (auto it = container.find(std::string(key)); it != container.end()) {
        return it->second.get();
    }

    auto* resource = index_->resource<Resource>(key);

    if (!resource) {
        LOG_ERROR("Unknown resource {}", key);
        return nullptr;
    }

    std::unique_ptr<Target> res = loader(*resource);

    if (!res) {
        LOG_ERROR("Resource {} failed to load", key);
        return nullptr;
    }

    Target* ptr = res.get();
    container[std::string(key)] = std::move(res);
    LOG_INFO("Resource {} loaded", key);
    return ptr;
}

Shader* ResourceManager::shader(std::string_view key) {
    return target<Shader, ShaderResource>(
        key,
        shaders_,
        [this](const ShaderResource& shader_resource) { return load_shader(shader_resource); }
    );
}

std::unique_ptr<Shader> ResourceManager::load_shader(const ShaderResource& shader_resource) const {
    const auto id = shader_resource.id();

    auto shader = std::make_unique<Shader>();

    for (const auto& [stage, path] : shader_resource.stage_paths()) {
        auto data = repository_->load_from_path(path);

        if (!data.has_value()) {
            LOG_ERROR("Could not load stage data of shader {} ", stage, id);
            return nullptr;
        }

        if (!shader->add_stage(stage, data.value())) {
            LOG_ERROR("Could not add stage {} to shader {}", stage, id);
            return nullptr;
        }
    }

    if (!shader->link_program()) {
        LOG_ERROR("Could not load shader {}: shader linking failed", id);
        return nullptr;
    }

    return shader;
}
