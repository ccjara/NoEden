#ifndef NOEDEN_SHADER_HXX
#define NOEDEN_SHADER_HXX

enum class ShaderStage {
    Vertex,
    Fragment,
    Geometry,
};

class Shader {
public:
    Shader();
    ~Shader();

    bool add_stage(ShaderStage type, const std::string& source);
    bool add_stage(ShaderStage type, const std::vector<u8>& data);
    bool link_program();
    void use() const;

    void set_uniform(const std::string& name, i32 value);
    void set_uniform(const std::string& name, f32 value);
    void set_uniform(const std::string& name, glm::ivec2 value);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

private:
    GLuint program_;
    std::vector<GLuint> shaders_;
    std::unordered_map<std::string, GLint> uniforms_;

    GLuint compile_shader(ShaderStage type, const std::string& source);
    GLint get_uniform_location(const std::string& name);

    GLenum gl_shader_type(ShaderStage type) const;
};

template <>
struct fmt::formatter<ShaderStage> : formatter<std::string> {
    auto format(ShaderStage c, format_context& ctx) const {
        std::string name;
        switch (c) {
        case ShaderStage::Vertex:
            name = "Vertex";
            break;
        case ShaderStage::Fragment:
            name = "Fragment";
            break;
        case ShaderStage::Geometry:
            name = "Geometry";
            break;
        default:
            name = std::to_string(static_cast<i32>(c));
            break;
        }
        return formatter<std::string>::format(name, ctx);
    }
};

#endif
