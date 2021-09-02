#ifndef JARALYN_PLATFORM_EVENT_HXX
#define JARALYN_PLATFORM_EVENT_HXX

#include "../env/root_config.hxx"
#include "../core/input/input_state.hxx"

/**
 * @brief Triggered on SDL_QUIT when closing the window
 *
 * This may occur when Alt+F4 is pressed or the X window
 * button is closed.
 */
struct j_quit_event {
};

/**
 * @brief Triggered as the user resizes the window.
 */
struct j_resize_event {
    j_vec2<uint32_t> size;
};

/**
 * @brief Triggered as the user moves the mouse inside the window.
 */
struct MouseMoveEvent {
    j_vec2<int32_t> position;
    const InputState* state;

    MouseMoveEvent(j_vec2<int32_t> position, InputState* state) :
        position { position }, state { state } {
        assert(state);
    }
};

/**
 * @brief Triggered clicking a mouse button (only when inside the window)
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
 * @brief Triggered when releasing a mouse button (only when inside the window)
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
 * @brief Triggered when pressing a key (only if the window is focused)
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
 * @brief Triggered when releasing a key (only if the window is focused)
 */
struct KeyUpEvent {
    Key key;
    const InputState* state;

    KeyUpEvent(Key key, InputState* state) :
        key { key }, state { state } {
        assert(state);
    }
};

/**
 * @brief Triggered every time the root configuration updates (usually from within lua)
 */
struct j_root_config_updated_event {
    j_root_config prev;
    j_root_config next;

    j_root_config_updated_event(j_root_config cfg_prev, j_root_config cfg_next) :
        prev(std::move(cfg_prev)),
        next(std::move(cfg_next)) {
    }
};

#endif
