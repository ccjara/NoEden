#ifndef NOEDEN_TEXTURE_HXX
#define NOEDEN_TEXTURE_HXX

/**
 * @brief Wrapper around a GL texture
 */
class Texture {
private:
    GLuint id_ { 0 };
    Vec2<u32> size_;
public:
    GLuint id() const;

    Texture() = default;
    ~Texture();

    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

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
    Vec2<u32> size() const;
};

#endif
