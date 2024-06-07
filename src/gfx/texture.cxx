#include "gfx/texture.hxx"
#include "gfx/image.hxx"

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

    const auto image = Image::from_file(path);

    glGenTextures(1, &id_);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        image.width(),
        image.height(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image.data()
    );

    const auto glError = glGetError();
    if (glError) {
        Log::error("Could not generate texture: {}", glError);
        std::abort();
    }
    unbind();

    size_ = { static_cast<u32> (image.width()), static_cast<u32> (image.height()) };
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
