#include "mouse.hxx"

j_position<int32_t> j_mouse::position() const noexcept {
    return position_;
}

void j_mouse::move(j_position<int32_t> pos) noexcept {
    position_ = pos;
}

void j_mouse::mouse_down(j_mouse_button button) noexcept {
    button_state_[button] = true;
}

void j_mouse::mouse_up(j_mouse_button button) noexcept {
    button_state_[button] = false;
}

bool j_mouse::is_pressed(j_mouse_button button) const noexcept {
    return button_state_.at(button);
}

int32_t j_mouse::x() const noexcept {
    return position_.x;
}

int32_t j_mouse::y() const noexcept {
    return position_.y;
}
