#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

/**
 * @brief Represents the game window by owning an SDL_Window
 *
 * Only a single window will be managed by the engine and we don't expect it
 * to be moved or even copied around
 */
class Window {
public:
    Window() = default;

    /**
     * @brief Frees the SDL window handle
     */
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    /**
     * @brief Implicit conversion to the managed handle
     */
    operator SDL_Window* () const;

    /**
     * @brief Updates the window size
     *
     * Does **not** resize the window programmatically. This is currently not
     * supported. The window size is backtracked when the window is resized
     * by the user using events.
     *
     * @see ResizeEvent
     */
    void resize(Vec2<u32> s);

    /**
     * @brief Creates the SDL managed window
     */
    void open(Vec2<u32> size, const char *title);

    /**
     * @brief Destroys the underlying SDL managed window
     */
    void close();

    [[nodiscard]] SDL_Window* handle() const;
    [[nodiscard]] Vec2<u32> size() const;
private:
    Vec2<u32> size_;
    std::string title_;
    SDL_Window* handle_ { nullptr };
};

#endif
