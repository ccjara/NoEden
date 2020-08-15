#ifndef JARALYN_WINDOW_HXX
#define JARALYN_WINDOW_HXX

/**
 * @brief Represents the game window by owning an SDL_Window
 *
 * Ideally only one should exist at any given time, but its
 * managed resources can be moved when such a use case arises.
 */
class j_window {
private:
    j_vec2<uint32_t> size_;
    SDL_Window* handle_ { nullptr };
public:
    /**
     * @brief Creates an SDL window with the given size
     */
    explicit j_window(j_vec2<uint32_t> size);

    /**
     * @brief Frees the SDL window handle
     */
    ~j_window() noexcept;

    // resource management, handle can only be moved
    j_window(const j_window&) = delete;
    j_window(j_window&&) noexcept;
    j_window& operator=(const j_window&) = delete;
    j_window& operator=(j_window&&) noexcept;

    /**
     * @brief Implicit conversion to the managed handle
     */
    operator SDL_Window* () const noexcept;

    /**
     * @brief Updates the window size
     *
     * Does **not** resize the window programmatically. This is currently not
     * supported. The window size is backtracked when the window is resized
     * by the user using env events.
     *
     * @see j_resize_event
     */
    void resize(j_vec2<uint32_t> s) noexcept;

    [[nodiscard]] SDL_Window* handle() const noexcept;
    [[nodiscard]] j_vec2<uint32_t> size() const noexcept;
};

#endif
