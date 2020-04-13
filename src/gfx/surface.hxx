#ifndef JARALYN_SURFACE_HXX
#define JARALYN_SURFACE_HXX

class j_surface {
private:
    SDL_Surface* surf_ { nullptr };
    j_size<uint32_t> size_;
public:
    j_surface() = default;

    explicit j_surface(const std::string& path);

    void load(const std::string& path);

    void* data() const noexcept;

    uint32_t width() const noexcept;
    uint32_t height() const noexcept;
    j_size<uint32_t> size() const noexcept;

    ~j_surface();
};

#endif
