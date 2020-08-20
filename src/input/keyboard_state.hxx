#ifndef JARALYN_KEYBOARD_STATE_HXX
#define JARALYN_KEYBOARD_STATE_HXX

/**
 * @brief Represents the current keyboard state
 */
class j_keyboard_state {
public:
    /**
     * @brief Returns true if the given key is pressed
     */
    virtual bool is_pressed(SDL_Keycode k) const noexcept = 0;

    /**
     * @brief Returns true if the given key is pressed and resets its state
     */
    virtual bool consume(SDL_Keycode k) noexcept = 0;
};

#endif
