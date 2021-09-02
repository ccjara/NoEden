#include "mouse_state.hxx"

j_vec2<int32_t> MouseState::position() const {
    return position_;
}

void MouseState::move(j_vec2<int32_t> pos) {
    position_ = pos;
}

void MouseState::mouse_down(MouseButton button) {
    button_state_[button_to_index(button)] = true;
}

void MouseState::mouse_up(MouseButton button) {
    button_state_[button_to_index(button)] = false;
}

bool MouseState::is_pressed(MouseButton button) const {
    return button_state_.at(button_to_index(button));
}

int32_t MouseState::x() const {
    return position_.x;
}

int32_t MouseState::y() const {
    return position_.y;
}

constexpr uint32_t MouseState::button_to_index(MouseButton button) const {
    return static_cast<uint32_t>(button) % MOUSE_BUTTON_COUNT;
}