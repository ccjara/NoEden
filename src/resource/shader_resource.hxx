#ifndef NOEDEN_SHADER_RESOURCE_HXX
#define NOEDEN_SHADER_RESOURCE_HXX

#include "gfx/shader.hxx"
#include "resource/resource.hxx"

class ShaderResource : public Resource {
public:
    explicit ShaderResource(std::string_view id);

    /**
     * @brief Adds the path to a SPIR-V file for the given stage
     */
    void add_stage_path(ShaderStage stage, const std::string& path);

    /**
     * @brief Adds the path to a SPIR-V file for the given stage
     */
    void add_stage_path(ShaderStage stage, std::string&& path);

    /**
     * @brief Returns the path to the SPIR-V file for the given stage
     */
    [[nodiscard]] std::optional<std::string_view> stage_path(ShaderStage stage) const;

    /**
     * @brief Returns the stage path dictioanry
     */
    [[nodiscard]] const std::unordered_map<ShaderStage, std::string>& stage_paths() const {
        return stage_paths_;
    }
private:
    std::unordered_map<ShaderStage, std::string> stage_paths_ = {};
};

#endif
