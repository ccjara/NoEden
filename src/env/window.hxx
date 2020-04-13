#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

class j_window {
private:
    j_size<uint32_t> size_;
    SDL_Window* handle_ { nullptr };
public:
    explicit j_window(j_size<uint32_t> size);
    ~j_window() noexcept;

    j_window(const j_window&) = delete;
    j_window(j_window&&) noexcept;

    j_window& operator=(const j_window&) = delete;
    j_window& operator=(j_window&&) noexcept;

    [[nodiscard]] SDL_Window* handle() const noexcept;
    [[nodiscard]] j_size<uint32_t> size() const noexcept;

    void resize(j_size<uint32_t> s) noexcept;
};

#endif
