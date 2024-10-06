#include "gfx/shader.hxx"
#include "catalog/catalog.hxx"
#include "catalog/material_reader.hxx"
#include "resource/resource_index.hxx"
#include "resource/resource_manager.hxx"
#include "resource/catalog_resource.hxx"
#include "resource/plain_resource_repository.hxx"
#include "resource/shader_resource.hxx"

ResourceManager::ResourceManager(ServiceLocator& svc) : svc_(svc) {

}

ResourceManager::~ResourceManager() {

}

bool ResourceManager::initialize() {
    tp_ = svc_.get<ThreadPool>();

    if (!tp_) {
        LOG_ERROR("Could not get ThreadPool");
        return false;
    }

    constexpr auto INDEX_TOML_PATH = "resources/index.toml";

    repository_ = std::make_unique<PlainResourceRepository>();
    index_ = std::make_unique<ResourceIndex>();

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

std::optional<toml::table> ResourceManager::toml_from_path(std::string_view path) const {
    const auto catalog_file_raw = repository_->load_from_path(path);

    if (!catalog_file_raw) {
        LOG_ERROR("Could not load toml file {}", path);
        return std::nullopt;
    }

    const std::string_view toml(
        reinterpret_cast<const char*>(catalog_file_raw->data()),
        catalog_file_raw->size()
    );

    auto parse_result = toml::parse(toml);

    if (!parse_result) {
        LOG_ERROR("Failed to parse loaded toml file {}: {}", path, parse_result.error().description());
        return std::nullopt;
    }

    return parse_result.table();
}

Catalog* ResourceManager::catalog() {
    if (catalog_) {
        return catalog_.get();
    }

    const auto* resource = index_->resource<CatalogResource>(CatalogResource::compound_id);

    if (!resource) {
        LOG_ERROR("Could not access resource {} at resource index", CatalogResource::compound_id);
        return nullptr;
    }

    const auto catalog_file_paths = repository_->list_files(resource->path(), ".toml");

    MaterialReader material_reader;
    std::mutex material_reader_mutex;

    TaskGroup tg_load_catalog_files;

    for (const auto& catalog_file_path : catalog_file_paths) {
        const auto result = tp_->run(tg_load_catalog_files, [&] {
            const auto catalog_file_table = toml_from_path(catalog_file_path);

            if (!catalog_file_table) {
                return;
            }

            auto root = catalog_file_table.value();

            // determine file type
            const auto file_table = root["file"].as_table();
            if (!file_table) {
                LOG_ERROR("Catalog file {} does not contain a 'file' table", catalog_file_path);
                return;
            }
            const auto type = file_table->get("type")->as_string();
            if (!type) {
                LOG_ERROR("File table of catalog file {} does not contain a 'type' field", catalog_file_path);
                return;
            }
            if (*type == "material") {
                std::lock_guard lock(material_reader_mutex);
                material_reader.add(catalog_file_path, std::move(root));
            }
        });

        if (result != EnqueueResult::Ok) {
            LOG_ERROR("Could not enqueue loading task for catalog file {}", catalog_file_path);
            return nullptr;
        }
    }

    if (!tg_load_catalog_files.await()) {
        LOG_ERROR("At least one catalog file loading task failed");
        return nullptr;
    }

    catalog_ = std::make_unique<Catalog>();
    catalog_->set_materials(material_reader.read_materials());

    LOG_INFO("Catalog loaded");

    return catalog_.get();
}
