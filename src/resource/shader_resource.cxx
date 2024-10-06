#include "gfx/shader.hxx"
#include "resource/shader_resource.hxx"

ShaderResource::ShaderResource(std::string_view id) : Resource(ResourceType::Shader, id, std::string_view("")) {
}

void ShaderResource::add_stage_path(ShaderStage stage, const std::string& path) {
    stage_paths_[stage] = path;
}

void ShaderResource::add_stage_path(ShaderStage stage, std::string&& path) {
    stage_paths_[stage] = std::move(path);
}

std::optional<std::string_view> ShaderResource::stage_path(ShaderStage stage) const {
    if (auto it = stage_paths_.find(stage); it != stage_paths_.end()) {
        return it->second;
    }

    return std::nullopt;
}
