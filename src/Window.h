#ifndef __WINDOW_H__
#define __WINDOW_H__

using WindowSizeType = uint32_t;
using WindowSize = Vector2<WindowSizeType>;

class Window
{
private:
    WindowSize size;
    SDL_Window* handle;
public:
    Window(const WindowSize& windowSize);
    ~Window();

    SDL_Window* getHandle() const noexcept;
    const WindowSize getSize() const noexcept;
    void adjust(const WindowSize& s) noexcept;
};

#endif
