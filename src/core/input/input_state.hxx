#ifndef JARALYN_INPUT_STATE_HXX
#define JARALYN_INPUT_STATE_HXX

#include "mouse_state.hxx"
#include "keyboard_state.hxx"

/**
 * @brief Readonly input state provider
 */
class InputState {
    friend class Input;
public:
    /**
     * @brief Returns the current mouse position as a tuple
     */
    j_vec2<uint32_t> mouse_position() const;

    /**
     * @brief Returns the current x coordinate of the mouse cursor
     */
    uint32_t mouse_x() const;

    /**
     * @brief Returns the current y coordinate of the mouse cursor
     */
    uint32_t mouse_y() const;

    /**
     * @brief Returns true if the given mouse button is currently pressed
     */
    bool is_mouse_pressed(MouseButton button) const;

    /**
     * @brief Returns true if the given keyboard key is currently pressed
     */
    bool is_key_pressed(Key key) const;
private:
    MouseState mouse_;
    KeyboardState keyboard_;
};

#endif
