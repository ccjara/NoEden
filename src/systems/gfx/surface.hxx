#ifndef JARALYN_SURFACE_HXX
#define JARALYN_SURFACE_HXX

class surface {
private:
    SDL_Surface* surf_ { nullptr };
    size<uint32_t> size_;
public:
    surface() = default;

    explicit surface(const std::string& path);

    void load(const std::string& path);

    void* data() const noexcept;

    uint32_t width() const noexcept;
    uint32_t height() const noexcept;
    size<uint32_t> get_size() const noexcept;

    ~surface();
};

#endif
