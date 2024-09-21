#pragma once

#include "framework/event_type.hxx"

/**
 * @brief Triggered as the user moves the mouse inside the window.
 */
struct MouseMoveEvent : Event<EventType::MouseMove> {
    glm::ivec2 position = {0, 0};

    const InputState& input_state;

    explicit MouseMoveEvent(const glm::ivec2& position, const InputState& input_state) :
        position(position), input_state(input_state) {}
};

/**
 * @brief Triggered clicking a mouse button (only when inside the window).
 */
struct MouseButtonDownEvent : Event<EventType::MouseButtonDown> {
    MouseButton button = MouseButton::Unknown;

    const InputState& input_state;

    explicit MouseButtonDownEvent(MouseButton button, const InputState& input_state) :
        button(button), input_state(input_state) {}
};

/**
 * @brief Triggered when releasing a mouse button (only when inside the window).
 */
struct MouseButtonUpEvent : Event<EventType::MouseButtonUp> {
    MouseButton button = MouseButton::Unknown;

    const InputState& input_state;

    explicit MouseButtonUpEvent(MouseButton button, const InputState& input_state) :
        button(button), input_state(input_state) {}
};

/**
 * @brief Triggered when pressing a key (only if the window is focused).
 */
struct KeyDownEvent : Event<EventType::KeyDown> {
    Key key = Key::Unknown;

    const InputState& input_state;

    explicit KeyDownEvent(Key key, const InputState& input_state) : key(key), input_state(input_state) {}
};

/**
 * @brief Triggered when releasing a key (only if the window is focused).
 */
struct KeyUpEvent : Event<EventType::KeyUp> {
    Key key = Key::Unknown;

    const InputState& input_state;

    explicit KeyUpEvent(Key key, const InputState& input_state) : key(key), input_state(input_state) {}
};
