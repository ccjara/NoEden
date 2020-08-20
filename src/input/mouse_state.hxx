#ifndef JARALYN_MOUSE_STATE_HXX
#define JARALYN_MOUSE_STATE_HXX

/**
 * @brief Represents the current mouse state
 */
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

#endif
