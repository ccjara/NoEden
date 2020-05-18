#ifndef JARALYN_SURFACE_HXX
#define JARALYN_SURFACE_HXX

class j_surface {
private:
    SDL_Surface* surf_ { nullptr };
    j_size<uint32_t> size_;
public:
    explicit j_surface(std::string_view path);

    j_surface() = default;
    j_surface(const j_surface&) = delete;
    j_surface(j_surface&&);
    j_surface& operator=(j_surface&&);
    j_surface& operator=(const j_surface&) = delete;
    ~j_surface();

    void load(std::string_view path);

    void* data() const noexcept;

    uint32_t width() const noexcept;
    uint32_t height() const noexcept;
    j_size<uint32_t> size() const noexcept;
};

#endif
