#ifndef JARALYN_INPUT_EVENT_HXX
#define JARALYN_INPUT_EVENT_HXX

#include "../input/input_state.hxx"

/**
 * @brief Triggered as the user moves the mouse inside the window.
 */
struct MouseMoveEvent {
    Vec2<i32> position;
    const InputState* state;

    MouseMoveEvent(Vec2<i32> position, InputState* state) :
        position { position }, state { state } {
        assert(state);
    }
};

/**
 * @brief Triggered clicking a mouse button (only when inside the window).
 */
struct MouseDownEvent {
    MouseButton button;
    const InputState* state;

    MouseDownEvent(MouseButton button, InputState* state) :
        button { button }, state { state } {
        assert(state);
    }
};

/**
 * @brief Triggered when releasing a mouse button (only when inside the window).
 */
struct MouseUpEvent {
    MouseButton button;
    const InputState* state;

    MouseUpEvent(MouseButton button, InputState* state) :
        button { button }, state { state } {
        assert(state);
    }
};

/**
 * @brief Triggered when pressing a key (only if the window is focused).
 */
struct KeyDownEvent {
    Key key;
    const InputState* state;

    KeyDownEvent(Key key, InputState* state) :
        key { key }, state { state } {
        assert(state);
    }
};

/**
 * @brief Triggered when releasing a key (only if the window is focused).
 */
struct KeyUpEvent {
    Key key;
    const InputState* state;

    KeyUpEvent(Key key, InputState* state) :
        key { key }, state { state } {
        assert(state);
    }
};

#endif
