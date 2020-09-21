#ifndef JARALYN_KEYBOARD_STATE_HXX
#define JARALYN_KEYBOARD_STATE_HXX

/**
 * @brief Represents the current keyboard state
 */
class j_keyboard_state {
public:
    virtual ~j_keyboard_state() = default;

    /**
     * @brief Returns true if the given key is pressed
     */
    virtual bool is_pressed(SDL_Keycode k) const = 0;

    /**
     * @brief Returns true if the given key is pressed and resets its state
     */
    virtual bool consume(SDL_Keycode k) = 0;
};

#endif
