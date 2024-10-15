#include "resource/catalog_resource.hxx"
#include "resource/resource_index.hxx"
#include "resource/shader_resource.hxx"

ResourceIndex::~ResourceIndex() {
}

Resource* ResourceIndex::resource(std::string_view key, ResourceType type) {
    if (auto it = resources_.find(std::string(key)); it != resources_.end()) {
        const auto actual_type = it->second->type();

        if (actual_type != type) {
            LOG_ERROR(
                "Tried to retrieve resource {} as type {} but it is indexed as type {}",
                key,
                static_cast<i32>(type),
                static_cast<i32>(actual_type)
                );
            return nullptr;
        }

        return it->second.get();
    }

    return nullptr;
}

bool ResourceIndex::load(std::istream& stream) {
    if (!stream) {
        LOG_ERROR("Could not load resource index: bad stream");
    }

    std::string json((std::istreambuf_iterator(stream)), std::istreambuf_iterator<char>());
    const auto parse_result = edenjson::parse(json);

    if (!parse_result) {
        LOG_ERROR("Could not parse resource index: {} (#{})", parse_result.error.message, parse_result.error.line);
        return false;
    }

    if (!index_catalog(parse_result.document)) {
        return false;
    }

    if (!index_shaders(parse_result.document)) {
        return false;
    }

    return true;
}

bool ResourceIndex::index_catalog(const edenjson::json_value& root) {
    const auto path = root[CatalogResource::compound_id].as_string().value_or("");

    if (path.empty()) {
        LOG_ERROR("Corrupted resource index: `catalog` must have a non-empty `path` string property");
        return false;
    }

    const auto [_, inserted] = resources_.try_emplace(
        std::string(CatalogResource::compound_id),
        std::make_unique<CatalogResource>(path)
        );

    if (!inserted) {
        LOG_ERROR("Catalog is already indexed");
        return false;
    }

    return true;
}


bool ResourceIndex::index_shaders(const edenjson::json_value& root) {
    const auto& shaders = root["shaders"].as_object().value_or(edenjson::json_object{});

    for (const auto& [k, v] : shaders) {
        if (!index_shader(k, v)) {
            LOG_ERROR("Failed to index shader {}", k);
            return false;
        }
    }

    return true;
}

bool ResourceIndex::index_shader(std::string_view resource_id, const edenjson::json_value& shader_entry) {
    assert(shader_entry.is_object());
    auto shader = std::make_unique<ShaderResource>(resource_id);

    for (const auto& [stage_key, v] : shader_entry.as_object().value()) {
        ShaderStage stage;
        if (stage_key == "vertex") {
            stage = ShaderStage::Vertex;
        } else if (stage_key == "fragment") {
            stage = ShaderStage::Fragment;
        } else if (stage_key == "geometry") {
            stage = ShaderStage::Geometry;
        } else {
            LOG_ERROR("Unsupported stage {}", stage_key);
            return false;
        }

        std::string stage_source = v.as_string().value_or("");

        if (stage_source.empty()) {
            LOG_ERROR("Shader source must be a non-empty string");
            return false;
        }

        shader->add_stage_path(stage, std::move(stage_source));
    }

    const auto [_, inserted] = resources_.try_emplace(std::string(resource_id), std::move(shader));

    if (!inserted) {
        LOG_ERROR("Shader {} is already indexed", resource_id);
        return false;
    }

    return true;
}
