#include "input.hxx"

Vec2<u32> Input::mouse_position() {
    return mouse_.position();
}

u32 Input::mouse_x() {
    return mouse_.x();
}

u32 Input::mouse_y() {
    return mouse_.y();
}

bool Input::is_mouse_pressed(MouseButton button) {
    return mouse_.is_pressed(button);
}

bool Input::is_key_pressed(Key key) {
    return keyboard_.is_pressed(key);
}
