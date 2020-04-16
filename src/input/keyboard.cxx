#include "keyboard.hxx"

j_keyboard::j_keyboard() {

}

bool j_keyboard::is_pressed(SDL_Keycode key) const noexcept {
    const auto k { keys_.find(key) };

    return k == keys_.end() ? false : k->second;
}

void j_keyboard::key_down(SDL_Keycode key) {
    keys_[key] = true;
}

void j_keyboard::key_up(SDL_Keycode key) {
    keys_[key] = false;
}
