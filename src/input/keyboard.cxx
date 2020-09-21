#include "keyboard.hxx"

bool j_keyboard::consume(SDL_Keycode key) {
    const auto k { keys_.find(key) };

    const auto pressed { k == keys_.end() ? false : k->second };

    if (pressed) {
        k->second = false;
    }
    return pressed;
}

bool j_keyboard::is_pressed(SDL_Keycode key) const {
    const auto k { keys_.find(key) };

    return k == keys_.end() ? false : k->second;
}

void j_keyboard::key_down(SDL_Keycode key) {
    keys_[key] = true;
}

void j_keyboard::key_up(SDL_Keycode key) {
    keys_[key] = false;
}
