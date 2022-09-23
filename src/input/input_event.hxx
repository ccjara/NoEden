#ifndef JARALYN_INPUT_EVENT_HXX
#define JARALYN_INPUT_EVENT_HXX

#include "mouse_button.hxx"
#include "key.hxx"

/**
 * @brief Triggered as the user moves the mouse inside the window.
 */
struct MouseMoveEvent {
    Vec2<i32> position;

    explicit MouseMoveEvent(Vec2<i32> position) : position(position) {
    }
};

/**
 * @brief Triggered clicking a mouse button (only when inside the window).
 */
struct MouseDownEvent {
    MouseButton button;

    explicit MouseDownEvent(MouseButton button) : button(button) {
    }
};

/**
 * @brief Triggered when releasing a mouse button (only when inside the window).
 */
struct MouseUpEvent {
    MouseButton button;

    explicit MouseUpEvent(MouseButton button) : button(button) {
    }
};

/**
 * @brief Triggered when pressing a key (only if the window is focused).
 */
struct KeyDownEvent {
    Key key;

    explicit KeyDownEvent(Key key) : key(key) {
    }
};

/**
 * @brief Triggered when releasing a key (only if the window is focused).
 */
struct KeyUpEvent {
    Key key;

    explicit KeyUpEvent(Key key) : key(key) {
    }
};

#endif
