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

    auto result = toml::parse(stream);

    if (!result) {
        LOG_ERROR("Could not parse resource index: {}", result.error().description());
        return false;
    }

    index_shaders(result.table());

    return true;
}

void ResourceIndex::index_shaders(const toml::table& table) {
    auto shaders_table = table.get_as<toml::table>("shaders");
    if (!shaders_table) {
        return;
    }

    for (const auto& [k, v] : *shaders_table) {
        if (auto* stage_array = v.as_array(); stage_array) {
            index_shader(k, *stage_array);
        }
        else {
            LOG_ERROR("Corrupted shader index entry {}: Value must be an array of shader stages", k.str());
        }
    }
}

void ResourceIndex::index_shader(std::string_view resource_id, const toml::array& stage_array) {
    const auto size = stage_array.size();
    std::unique_ptr<ShaderResource> shader = std::make_unique<ShaderResource>(resource_id);

    for (size_t i = 0; i < size; ++i) {
        const auto& table = stage_array[i].as_table();

        if (!table) {
            LOG_ERROR(
                "Corrupted stage entry {} in shader resource index entry {}: Array element must be a table",
                i, resource_id);
            return;
        }

        const auto path_property = table->get_as<std::string>("path");
        if (!path_property) {
            LOG_ERROR(
                "Corrupted stage entry {} in shader resource index entry {}: Stage table must have a `path` string property",
                i, resource_id);
            return;
        }

        auto path_str = path_property->as_string()->get();
        if (path_str.empty()) {
            LOG_ERROR("Corrupted stage entry {} in shader resource index entry {}: Path is empty", i, resource_id);
            return;
        }

        const auto stage_property = table->get_as<std::string>("stage");
        if (!stage_property) {
            LOG_ERROR(
                "Corrupted stage entry {} in shader resource index entry {}: Stage table must have a `stage` string property",
                i, resource_id);
            return;
        }

        const auto stage_str = stage_property->as_string()->get();

        ShaderStage stage;
        if (stage_str == "vertex") {
            stage = ShaderStage::Vertex;
        } else if (stage_str == "fragment") {
            stage = ShaderStage::Fragment;
        } else if (stage_str == "geometry") {
            stage = ShaderStage::Geometry;
        } else {
            LOG_ERROR("Corrupted resource index entry {} in stage element {}: unsupported stage {}", resource_id, i,
                stage_str);
            return;
        }

        shader->add_stage_path(stage, std::move(path_str));
    }

    const auto [_, inserted] = resources_.try_emplace(std::string(resource_id), std::move(shader));

    if (!inserted) {
        LOG_ERROR("Shader {} is already indexed", resource_id);
    }
}
