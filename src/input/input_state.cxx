#include "input_state.hxx"

Vec2<u32> InputState::mouse_position() const {
    return mouse_.position();
}

u32 InputState::mouse_x() const {
    return mouse_.x();
}

u32 InputState::mouse_y() const {
    return mouse_.y();
}

bool InputState::is_mouse_pressed(MouseButton button) const {
    return mouse_.is_pressed(button);
}

bool InputState::is_key_pressed(Key key) const {
    return keyboard_.is_pressed(key);
}
