#include "input_state.hxx"

j_vec2<uint32_t> InputState::mouse_position() const {
    return mouse_.position();
}

uint32_t InputState::mouse_x() const {
    return mouse_.x();
}

uint32_t InputState::mouse_y() const {
    return mouse_.y();
}

bool InputState::is_mouse_pressed(MouseButton button) const {
    return mouse_.is_pressed(button);
}

bool InputState::is_key_pressed(Key key) const {
    return keyboard_.is_pressed(key);
}
