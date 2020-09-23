#ifndef JARALYN_MOUSE_HXX
#define JARALYN_MOUSE_HXX

#include "mouse_state.hxx"

class j_mouse : public j_mouse_state {
private:
    j_vec2<int32_t> position_;

    std::unordered_map<j_mouse_button, bool> button_state_;
public:
    j_vec2<int32_t> position() const override;

    int32_t x() const override;
    int32_t y() const override;

    /**
     * @brief Sets the mouse position (absolute)
     */
    void move(j_vec2<int32_t> pos);

    /**
     * @brief Updates the state of the given button to be pressed
     */
    void mouse_down(j_mouse_button button);

    /**
     * @brief Updates the state of the given button to be released
     */
    void mouse_up(j_mouse_button button);

    /**
     * @brief Returns whether the given button is currently pressed
     */
    bool is_pressed(j_mouse_button button) const override;
};

#endif
