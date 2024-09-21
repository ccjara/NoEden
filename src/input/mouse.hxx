#pragma once

using MousePosition = glm::ivec2;
using MouseDelta = glm::ivec2;

enum class MouseButton {
    Unknown = 0,
    Left = 1,
    Middle = 2,
    Right = 3,
    X1 = 4,
    X2 = 5,
};

constexpr i32 MOUSE_BUTTON_COUNT = 6;
