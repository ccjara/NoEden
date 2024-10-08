#ifndef NOEDEN_TEXTURE_HXX
#define NOEDEN_TEXTURE_HXX

/**
 * @brief Wrapper around a GL texture
 */
class Texture {
private:
    GLuint id_ { 0 };
    glm::ivec2 size_ = { 0, 0 };
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
     */
    [[nodiscard]] bool load(std::string_view path);

    /**
     * @briefs Frees the managed GL resource.
     */
    void unload();

    /**
     * @briefs Binds the texture for the next GL draw operation
     */
    void bind() const;

    /**
     * @briefs Unbinds the currently active texture for the next GL draw operation
     *
     * Note that as this calls glBindTexture(0) this will unbind any texture resource,
     * regardless whether it is managed by this wrapper.
     */
    void unbind() const;

    bool is_loaded() const;
    glm::ivec2 size() const;
};

#endif
