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

bool Texture::load(std::string_view path) {
    unload();

    const auto image = Image::from_file(path);

    if (!image.loaded()) {
        LOG_ERROR("Could not load image from path {}", path);
        return false;
    }

    glGenTextures(1, &id_);
    GL_CHECK(fmt::format("Could not generate texture {}", path));
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

    GL_CHECK(fmt::format("Could not map texture {}", path));

    unbind();

    size_ = { image.width(), image.height() };

    return true;
}

GLuint Texture::id() const {
    return id_;
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

glm::ivec2 Texture::size() const {
    return size_;
}
