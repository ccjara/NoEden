#include "mouse.hxx"

j_vec2<int32_t> j_mouse::position() const {
    return position_;
}

void j_mouse::move(j_vec2<int32_t> pos) {
    position_ = pos;
}

void j_mouse::mouse_down(j_mouse_button button) {
    button_state_[button] = true;
}

void j_mouse::mouse_up(j_mouse_button button) {
    button_state_[button] = false;
}

bool j_mouse::is_pressed(j_mouse_button button) const {
    return button_state_.at(button);
}

int32_t j_mouse::x() const {
    return position_.x;
}

int32_t j_mouse::y() const {
    return position_.y;
}
