#include "gfx/shader.hxx"

Shader::Shader() {
    program_ = glCreateProgram();
}

Shader::~Shader() {
    for (GLuint shader : shaders_) {
        glDeleteShader(shader);
    }
    if (program_ != 0) {
        glDeleteProgram(program_);
    }
}

bool Shader::add_stage(ShaderStage type, const std::string& source) {
    GLuint shader = compile_shader(type, source);
    if (shader == 0) {
        return false;
    }
    shaders_.push_back(shader);
    glAttachShader(program_, shader);
    return true;
}

bool Shader::add_stage(ShaderStage type, const std::vector<u8>& data) {
    const auto gl_type = gl_shader_type(type);
    if (gl_type == 0) {
        return false;
    }
    GLuint shader = glCreateShader(gl_type);

    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.data(), data.size());
    glSpecializeShader(shader, "main", 0, nullptr, nullptr);
    glAttachShader(program_, shader);

    shaders_.push_back(shader);

    return true;
}


GLuint Shader::compile_shader(ShaderStage type, const std::string& source) {
    const auto gl_type = gl_shader_type(type);
    if (gl_type == 0) {
        return 0;
    }
    GLuint shader = glCreateShader(gl_type);
    const char* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        LOG_ERROR("Shader compilation failed: {}", infoLog.data());

        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

bool Shader::link_program() {
    glLinkProgram(program_);

    GLint is_linked = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &is_linked);
    if (is_linked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program_, maxLength, &maxLength, &infoLog[0]);

        LOG_ERROR("Shader linking failed: {}", infoLog.data());

        glDeleteProgram(program_);
        program_ = 0;
        return false;
    }

    for (GLuint shader : shaders_) {
        glDetachShader(program_, shader);
    }
    shaders_.clear();

    return true;
}

void Shader::use() const {
    glUseProgram(program_);
}

void Shader::set_uniform(const std::string& name, i32 value) {
    glUniform1i(get_uniform_location(name), value);
    auto x = glGetError();
    if (x != GL_NO_ERROR) {
        LOG_ERROR("Failed to set uniform: {}", x);
    }
}

void Shader::set_uniform(const std::string& name, f32 value) {
    glUniform1f(get_uniform_location(name), value);
    auto x = glGetError();
    if (x != GL_NO_ERROR) {
        LOG_ERROR("Failed to set uniform: {}", x);
    }
}

void Shader::set_uniform(const std::string& name, Vec2<u32> value) {
    glUniform2ui(get_uniform_location(name), value.x, value.y);
    auto x = glGetError();
    if (x != GL_NO_ERROR) {
        LOG_ERROR("Failed to set uniform: {}", x);
    }
}

/*
void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
    glUniform2fv(get_uniform_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
    glUniform3fv(get_uniform_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
    glUniform4fv(get_uniform_location(name), 1, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
}
*/

GLint Shader::get_uniform_location(const std::string& name) {
    auto it = uniforms_.find(name);
    if (it != uniforms_.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(program_, name.c_str());
    if (location == -1) {
        LOG_ERROR("Uniform {} not found in shader.", name);
    }
    uniforms_[name] = location;
    return location;
}

GLenum Shader::gl_shader_type(ShaderStage type) const {
    switch (type) {
    case ShaderStage::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderStage::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderStage::Geometry:
        return GL_GEOMETRY_SHADER;
    default:
        LOG_ERROR("Unknown shader type: {}", static_cast<i32>(type));
        return 0;
    }
}
