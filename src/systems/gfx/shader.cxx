#include "shader.hxx"

shader::~shader() {
    if (is_loaded()) {
        glDeleteProgram(program_);
    }
}

void shader::load_source(
    const std::string& vertex_source,
    const std::string& geometry_source,
    const std::string& fragment_source
) {
    if (is_loaded()) {
        glDeleteProgram(program_);
    }
    program_ = glCreateProgram();
    if (!program_) {
        LOG(ERROR) << "Could not create shader program";
        throw;
    }

    const auto vshader { glCreateShader(GL_VERTEX_SHADER) };
    const auto gshader { glCreateShader(GL_GEOMETRY_SHADER) };
    const auto fshader { glCreateShader(GL_FRAGMENT_SHADER) };
    GLint ok { 0 };

    std::array<char, 512> info;

    const auto vss { vertex_source.data() };
    const auto gss { geometry_source.data() };
    const auto fss { fragment_source.data() };

    glShaderSource(vshader, 1, &vss, nullptr);
    glCompileShader(vshader);
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(vshader, info.size(), NULL, info.data());
        LOG(ERROR) << "Could not compile vertex shader: " << info.data();
        throw;
    }

    glShaderSource(gshader, 1, &gss, nullptr);
    glCompileShader(gshader);
    glGetShaderiv(gshader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(gshader, info.size(), NULL, info.data());
        LOG(ERROR) << "Could not compile geometry shader: " << info.data();
        throw;
    }

    glShaderSource(fshader, 1, &fss, nullptr);
    glCompileShader(fshader);
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(fshader, info.size(), NULL, info.data());
        LOG(ERROR) << "Could not compile fragment shader: " << info.data();
        throw;
    }

    glAttachShader(program_, vshader);
    glAttachShader(program_, gshader);
    glAttachShader(program_, fshader);

    glLinkProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &ok);
    if (!ok) {
        LOG(ERROR) << "Could not link shader program";
        throw;
    }

    glDeleteShader(vshader);
    glDeleteShader(gshader);
    glDeleteShader(fshader);
}

void shader::unload() {
    if (is_loaded()) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void shader::use() noexcept {
    prepare();

    glUseProgram(program_);
}

void shader::prepare() {

}

bool shader::is_loaded() const noexcept {
    return !!program_;
}

GLint shader::id() const noexcept {
    return program_;
}
