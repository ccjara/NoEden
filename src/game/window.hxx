#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

/**
 * @brief Represents the game window by owning an SDL_Window
 */
class Window {
public:
    Window() = delete;

    /**
     * @brief Updates the window size
     *
     * Does **not** resize the window programmatically. This is currently not
     * supported. The window size is backtracked when the window is resized
     * by the user using events.
     *
     * @see ResizeEvent
     */
    static void resize(Vec2<u32> s);

    /**
     * @brief Creates the SDL managed window
     */
    static void open(Vec2<u32> size, const char *title);

    /**
     * @brief Destroys the underlying SDL managed window
     */
    static void close();

    [[nodiscard]] static SDL_Window* handle();
    [[nodiscard]] static Vec2<u32> size();
private:
    static inline Vec2<u32> size_;
    static inline std::string title_;
    static inline SDL_Window* handle_ = nullptr;
};

#endif
