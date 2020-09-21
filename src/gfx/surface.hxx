#ifndef JARALYN_SURFACE_HXX
#define JARALYN_SURFACE_HXX

/**
 * @brief Wrapper around an SDL_Surface, used in textures
 *
 * @see j_texture
 */
class j_surface {
private:
    SDL_Surface* surf_ { nullptr };
    j_vec2<uint32_t> size_;
public:
    /**
     * @brief Loads a surface immediately
     */
    explicit j_surface(std::string_view path);

    j_surface() = default;
    j_surface(const j_surface&) = delete;
    j_surface(j_surface&&);
    j_surface& operator=(j_surface&&);
    j_surface& operator=(const j_surface&) = delete;
    ~j_surface();

    /**
     * @brief Loads a surface from the given file path
     *
     * Currently only BMP is supported
     */
    void load(std::string_view path);

    /**
     * @brief Pointer to the raw surface buffer
     */
    void* data() const;

    uint32_t width() const;
    uint32_t height() const;
    j_vec2<uint32_t> size() const;
};

#endif
