#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

class window {
private:
    size<uint32_t> size_;
    SDL_Window* handle_ { nullptr };
public:
    explicit window(size<uint32_t> size);
    ~window() noexcept;

    window(const window&) = delete;
    window(window&&) noexcept;

    window& operator=(const window&) = delete;
    window& operator=(window&&) noexcept;

    [[nodiscard]] SDL_Window* get_handle() const noexcept;
    [[nodiscard]] size<uint32_t> get_size() const noexcept;

    void resize(size<uint32_t> s) noexcept;
};

#endif
