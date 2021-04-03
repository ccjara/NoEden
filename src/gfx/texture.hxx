#ifndef JARALYN_TEXTURE_HXX
#define JARALYN_TEXTURE_HXX

#include "surface.hxx"

/**
 * @brief Wrapper around a GL texture
 */
class j_texture {
private:
    GLuint id_ { 0 };
    j_vec2<uint32_t> size_;
public:
    GLuint id() const;

    j_texture() = default;
    ~j_texture();

    j_texture(j_texture&&) = delete;
    j_texture& operator=(j_texture&&) = delete;

    j_texture(const j_texture&) = delete;
    j_texture& operator=(const j_texture&) = delete;

    /**
     * @briefs Load a texture from the given path
     *
     * If a texture is already loaded, it will be unloaded beforehand.
     *
     * Note: Supports only BMP as of now.
     */
    void load(std::string_view path);

    /**
     * @briefs Frees the managed GL resource.
     */
    void unload();

    /**
     * @briefs Binds the texture for the next GL draw operation
     */
    void bind();

    /**
     * @briefs Unbinds the currently active texture for the next GL draw operation
     *
     * Note that as this calls glBindTexture(0) this will unbind any texture resource,
     * regardless whether it is managed by this wrapper.
     */
    void unbind();

    bool is_loaded() const;
    j_vec2<uint32_t> size() const;
};

#endif
