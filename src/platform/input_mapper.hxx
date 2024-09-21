#pragma once

/**
 * @brief Maps SDL input to the engine's input types.
 */
class InputMapper {
public:
    static MouseButton map_sdl_mouse_button(i32 button) {
        if (button <= 0 || button > MOUSE_BUTTON_COUNT) {
            return MouseButton::Unknown;
        }
        return static_cast<MouseButton>(button);
    }

    static Key map_sdl_key(SDL_Scancode scancode) {
        if (scancode <= 0 || scancode > KEY_COUNT) {
            return Key::Unknown;
        }
        return static_cast<Key>(scancode);
    }
};
