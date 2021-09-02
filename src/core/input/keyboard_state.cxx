#include "keyboard_state.hxx"

bool KeyboardState::consume(Key key) {
    const auto k { keys_.find(key) };
    const auto pressed { k == keys_.end() ? false : k->second };
    if (pressed) {
        k->second = false;
    }
    return pressed;
}

bool KeyboardState::is_pressed(Key key) const {
    const auto k { keys_.find(key) };

    return k == keys_.end() ? false : k->second;
}

void KeyboardState::key_down(Key key) {
    keys_[key] = true;
}

void KeyboardState::key_up(Key key) {
    keys_[key] = false;
}