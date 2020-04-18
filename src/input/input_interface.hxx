#ifndef JARALYN_INPUT_INTERFACE_HXX
#define JARALYN_INPUT_INTERFACE_HXX

class j_mouse_state {
public:
    /**
     * @brief Returns the current x position
     */
    virtual int32_t x() const noexcept = 0;

    /**
     * @brief Returns the current y position
     */
    virtual int32_t y() const noexcept = 0;

    /**
     * @brief Returns the current position
     */
    virtual j_vec2<int32_t> position() const noexcept = 0;

    /**
     * @brief Returns true if the given button is pressed
     */
    virtual bool is_pressed(j_mouse_button) const noexcept = 0;
};

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

class j_input_state {
public:
    /**
     * @brief Returns the current, readonly keyboard state
     */
    virtual j_keyboard_state& keyboard() noexcept = 0;

    /**
     * @brief Returns the current, readonly mouse state
     */
    virtual j_mouse_state& mouse() noexcept = 0;
};

#endif
