#ifndef JARALYN_SURFACE_HXX
#define JARALYN_SURFACE_HXX

/**
 * @brief Wrapper around an SDL_Surface, used in textures
 *
 * @see Texture
 */
class Surface {
private:
    SDL_Surface* surf_ { nullptr };
    Vec2<u32> size_;
public:
    /**
     * @brief Loads a surface immediately
     */
    explicit Surface(std::string_view path);

    Surface() = default;
    Surface(const Surface&) = delete;
    Surface(Surface&&);
    Surface& operator=(Surface&&);
    Surface& operator=(const Surface&) = delete;
    ~Surface();

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

    u32 width() const;
    u32 height() const;
    Vec2<u32> size() const;
};

#endif
