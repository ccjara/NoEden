#include "gfx/shader.hxx"
#include "catalog/catalog.hxx"
#include "catalog/entity_template_parser.hxx"
#include "catalog/material_parser.hxx"
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

    constexpr auto INDEX_JSON_PATH = "resources/index.json";

    repository_ = std::make_unique<PlainResourceRepository>();
    index_ = std::make_unique<ResourceIndex>();

    auto file = std::ifstream(INDEX_JSON_PATH);
    if (!file) {
        LOG_ERROR("Could not load resource index from path {}: could not open file for reading", INDEX_JSON_PATH);
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

std::optional<edenjson::json_value> ResourceManager::json_from_path(std::string_view path) const {
    const auto json_buffer = repository_->load_from_path(path);

    if (!json_buffer) {
        LOG_ERROR("Could not load json file {}", path);
        return std::nullopt;
    }

    const std::string_view json(
        reinterpret_cast<const char*>(json_buffer->data()),
        json_buffer->size()
    );

    auto result = edenjson::parse(json);

    if (!result) {
        LOG_ERROR("Failed to parse loaded json file {}: {} (#{})", path, result.error.message, result.error.line);
        return std::nullopt;
    }

    return std::move(result.document);
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

    const auto catalog_file_paths = repository_->list_files(resource->path(), ".json");

    std::unordered_map<std::string, edenjson::json_value> entity_template_files, material_files;

    std::mutex material_files_mutex;
    std::mutex entity_template_files_mutex;

    TaskGroup tg_load_catalog_files;

    for (const auto& catalog_file_path : catalog_file_paths) {
        const auto result = tp_->run(tg_load_catalog_files, [&] {
            auto root_opt = json_from_path(catalog_file_path);

            if (!root_opt) {
                return;
            }

            auto root = std::move(root_opt.value());

            // determine file type
            const auto& file_type = root["file"]["type"].as_string().value_or("");
            if (file_type.empty()) {
                LOG_ERROR("File type of catalog file {} is not specified", catalog_file_path);
                return;
            }
            if (file_type == "material") {
                std::lock_guard lock(material_files_mutex);
                material_files.emplace(catalog_file_path, std::move(root));
            } else if (file_type == "entity_template") {
                std::lock_guard lock(entity_template_files_mutex);
                entity_template_files.emplace(catalog_file_path, std::move(root));
            } else {
                LOG_ERROR("Unknown file type {} in catalog file {}", file_type, catalog_file_path);
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
    catalog_->set_materials(parse_materials(material_files));
    catalog_->set_entity_templates(parse_entity_templates({ *svc_.get<ConditionResolver>(), entity_template_files }));

    LOG_INFO("Catalog loaded");

    return catalog_.get();
}
