#include "shader.hxx"

j_shader::j_shader() {
    program_ = glCreateProgram();
}

j_shader::~j_shader() {
    unload();
    clear_stages();
}

bool j_shader::compile(j_shader_type type, std::string_view src) {
    if (stages_.find(type) != stages_.end()) {
        LOG(ERROR) << "Shader stage " << static_cast<int>(type) << " is already sourced";
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
        LOG(ERROR) << "Could not compile shader: " << info;
        return false;
    }
    glAttachShader(program_, shader);
    return true;
}

bool j_shader::link() {
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
        LOG(ERROR) << "Could not link shader: " << info;
        return false;
    }
    clear_stages();
    return true;
}

void j_shader::clear_stages() {
    if (stages_.empty()) {
        return;
    }
    for (const auto stage : stages_) {
        glDeleteShader(stage.second);
    }
    stages_.clear();
}

void j_shader::unload() {
    if (is_loaded()) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void j_shader::use() noexcept {
    prepare();

    glUseProgram(program_);
}

void j_shader::prepare() {

}

bool j_shader::is_loaded() const noexcept {
    return !!program_;
}

GLint j_shader::id() const noexcept {
    return program_;
}
