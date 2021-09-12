#include "shader.hxx"

Shader::Shader() {
    program_ = glCreateProgram();
}

Shader::~Shader() {
    unload();
    clear_stages();
}

bool Shader::compile(Shader_type type, std::string_view src) {
    if (stages_.find(type) != stages_.end()) {
        Log::error("Shader stage {} is already compiled", static_cast<u32>(type));
        return false;
    }
    const auto shader { glCreateShader(static_cast<GLenum>(type)) };
    stages_[type] = shader;

    const auto p_source { src.data() };
    int ok { 0 };
    glShaderSource(shader, 1, &p_source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint log_length { 0 };
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::string info;
        if (log_length) {
            info.reserve(log_length);
            glGetShaderInfoLog(shader, log_length, nullptr, info.data());
        } else {
            info = "No opengl error info is available";
        }
        Log::error("Could not compile shader stage {}: {}", type, info);
        return false;
    }
    glAttachShader(program_, shader);
    return true;
}

bool Shader::link() {
    int ok { 0 };

    glLinkProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint log_length { 0 };
        std::string info;
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length) {
            info.reserve(log_length);
            glGetProgramInfoLog(program_, info.size(), nullptr, &info[0]);
        }
        else {
            info = "No opengl error info is available";
        }
        Log::info("Could not link shader: {}", info);
        return false;
    }
    clear_stages();
    return true;
}

void Shader::clear_stages() {
    if (stages_.empty()) {
        return;
    }
    for (const auto stage : stages_) {
        glDeleteShader(stage.second);
    }
    stages_.clear();
}

void Shader::unload() {
    if (is_loaded()) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void Shader::use() {
    glUseProgram(program_);
    prepare();
}

void Shader::prepare() {

}

bool Shader::is_loaded() const {
    return !!program_;
}

GLint Shader::id() const {
    return program_;
}
