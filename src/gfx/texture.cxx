#include "texture.hxx"

j_texture::~j_texture() {
    unload();
}

void j_texture::unload() {
    if (is_loaded()) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}

bool j_texture::is_loaded() const noexcept {
    return !!id_;
}

void j_texture::load(std::string_view path) {
    unload();

    j_surface surf { path };

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        static_cast<GLsizei> (surf.width()),
        static_cast<GLsizei> (surf.height()),
        0,
        GL_BGR,
        GL_UNSIGNED_BYTE,
        surf.data()
    );

    const auto glError = glGetError();
    if (glError) {
        LOG(ERROR) << "Could not generate texture from surface (" << glError << ")";
        throw;
    }

    size_ = surf.size();
}

GLuint j_texture::id() const noexcept {
    return id_;
}

void j_texture::bind() noexcept {
    glBindTexture(GL_TEXTURE_2D, id_);
}

void j_texture::unbind() noexcept {
    glBindTexture(GL_TEXTURE_2D, 0);
}

j_size<uint32_t> j_texture::size() const noexcept {
    return size_;
}
