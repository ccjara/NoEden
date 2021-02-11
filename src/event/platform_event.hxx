#ifndef JARALYN_PLATFORM_EVENT_HXX
#define JARALYN_PLATFORM_EVENT_HXX

#include "../input/mouse_state.hxx"
#include "../env/root_config.hxx"

/**
 * @brief Triggered on SDL_QUIT when closing the window
 *
 * This may occur when Alt+F4 is pressed or the X window
 * button is closed.
 */
struct j_quit_event {
};

/**
 * @brief Triggered on SDL_WINDOWEVENT_RESIZED as the user resizes the window.
 */
struct j_resize_event {
    j_vec2<uint32_t> size;
};

/**
 * @brief Triggered on SDL_MOUSEMOTION as the user moves the mouse inside the window.
 */
struct j_mouse_move_event {
    j_vec2<int32_t> position;
};

/**
 * @brief Triggered on SDL_MOUSEBUTTONDOWN (only when inside the window)
 */
struct j_mouse_down_event {
    j_mouse_button button;
};

/**
 * @brief Triggered on SDL_MOUSEBUTTONUP (only when inside the window)
 */
struct j_mouse_up_event {
    j_mouse_button button;
};

/**
 * @brief Triggered on SDL_KEYDOWN (only when window is focused)
 */
struct j_key_down_event {
    SDL_Keycode key;
};

/**
 * @brief Triggered on SDL_KEYUP (only when window is focused)
 */
struct j_key_up_event {
    SDL_Keycode key;
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
