#pragma once

#include "input/key.hxx"
#include "input/mouse.hxx"

/**
 * @brief Input state buffer
 */
struct InputState {
    /**
     * @brief Returns true if the given mouse button is currently pressed
     */
    [[nodiscard]] bool mouse_button_pressed(MouseButton button) const {
        return mouse_buttons.test(static_cast<i32>(button));
    }

    /**
     * @brief Returns true if the given key is currently pressed
     */
    [[nodiscard]] bool key_pressed(Key key) const {
        return keys[static_cast<i32>(key)];
    }

    MousePosition mouse_position = {0, 0};
    MouseDelta mouse_delta = {0, 0};
    std::bitset<MOUSE_BUTTON_COUNT> mouse_buttons = {false};
    std::array<bool, KEY_COUNT> keys = {false};
};
