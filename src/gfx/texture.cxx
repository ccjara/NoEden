#include "texture.hxx"

Texture::~Texture() {
    unload();
}

void Texture::unload() {
    if (is_loaded()) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}

bool Texture::is_loaded() const {
    return !!id_;
}

void Texture::load(std::string_view path) {
    unload();

    Surface surf { path };

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
        Log::error("Could not generate texture from surface: {}", glError);
        std::abort();
    }

    size_ = surf.size();
}

GLuint Texture::id() const {
    return id_;
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

Vec2<u32> Texture::size() const {
    return size_;
}
