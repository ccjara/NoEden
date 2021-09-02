#ifndef JARALYN_MOUSE_STATE_HXX
#define JARALYN_MOUSE_STATE_HXX

#include "mouse_button.hxx"

/**
 * @brief Buffers the mouse state
 */
class MouseState {
public:
    j_vec2<int32_t> position() const;

    int32_t x() const;
    int32_t y() const;

    /**
     * @brief Sets the mouse position (absolute)
     */
    void move(j_vec2<int32_t> pos);

    /**
     * @brief Updates the state of the given button to `pressed`
     */
    void mouse_down(MouseButton button);

    /**
     * @brief Updates the state of the given button to `released`
     */
    void mouse_up(MouseButton button);

    /**
     * @brief Returns whether the given button is currently pressed
     */
    bool is_pressed(MouseButton button) const;
private:
    j_vec2<int32_t> position_;
    std::array<bool, MOUSE_BUTTON_COUNT> button_state_;

    constexpr uint32_t button_to_index(MouseButton button) const;
};

#endif
