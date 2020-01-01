#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

#include "ide_pch.hxx"

using window_size = vector2<uint32_t>;

class window
{
private:
    window_size size;
    SDL_Window* handle = nullptr;
public:
    explicit window(window_size size);
    ~window() noexcept;

    [[nodiscard]] SDL_Window* get_handle() const noexcept;
    [[nodiscard]] window_size get_size() const noexcept;
    void resize(window_size s) noexcept;
};

#endif
