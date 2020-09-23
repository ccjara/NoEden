#ifndef JARALYN_MOUSE_STATE_HXX
#define JARALYN_MOUSE_STATE_HXX

enum class j_mouse_button {
    left = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right = SDL_BUTTON_RIGHT,
};

/**
 * @brief Represents the current mouse state
 */
class j_mouse_state {
public:
    virtual ~j_mouse_state() = default;

    /**
     * @brief Returns the current x position
     */
    virtual int32_t x() const = 0;

    /**
     * @brief Returns the current y position
     */
    virtual int32_t y() const = 0;

    /**
     * @brief Returns the current position
     */
    virtual j_vec2<int32_t> position() const = 0;

    /**
     * @brief Returns true if the given button is pressed
     */
    virtual bool is_pressed(j_mouse_button) const = 0;
};

#endif
