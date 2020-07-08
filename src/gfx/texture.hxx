#ifndef JARALYN_TEXTURE_HXX
#define JARALYN_TEXTURE_HXX

#include "surface.hxx"

/**
 * @brief Wrapper around a GL texture
 */
class j_texture {
private:
    GLuint id_;

    j_size<uint32_t> size_;
public:
    GLuint id() const noexcept;

    ~j_texture();
    
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
    void bind() noexcept;

    /**
     * @briefs Unbinds the currently active texture for the next GL draw operation
     *
     * Note that as this calls glBindTexture(0) this will unbind any texture resource,
     * regardless whether it is managed by this wrapper.
     */
    void unbind() noexcept;

    bool is_loaded() const noexcept;
    j_size<uint32_t> size() const noexcept;
};

#endif
