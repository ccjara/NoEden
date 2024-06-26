#ifndef NOEDEN_INPUT_EVENT_HXX
#define NOEDEN_INPUT_EVENT_HXX

#include "framework/event_type.hxx"
#include "input/mouse_button.hxx"
#include "input/key.hxx"

/**
 * @brief Triggered as the user moves the mouse inside the window.
 */
struct MouseMoveEvent : public Event<EventType::MouseMove> {
    Vec2<i32> position;

    explicit MouseMoveEvent(const Vec2<i32>& position) : position(position) {
    }
};

/**
 * @brief Triggered clicking a mouse button (only when inside the window).
 */
struct MouseDownEvent : public Event<EventType::MouseDown> {
    MouseButton button;

    explicit MouseDownEvent(MouseButton button) : button(button) {
    }
};

/**
 * @brief Triggered when releasing a mouse button (only when inside the window).
 */
struct MouseUpEvent : public Event<EventType::MouseUp> {
    MouseButton button;

    explicit MouseUpEvent(MouseButton button) : button(button) {
    }
};

/**
 * @brief Triggered when pressing a key (only if the window is focused).
 */
struct KeyDownEvent : public Event<EventType::KeyDown> {
    Key key;

    explicit KeyDownEvent(Key key) : key(key) {
    }
};

/**
 * @brief Triggered when releasing a key (only if the window is focused).
 */
struct KeyUpEvent : public Event<EventType::KeyUp> {
    Key key;

    explicit KeyUpEvent(Key key) : key(key) {
    }
};

#endif
