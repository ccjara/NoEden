#ifndef JARALYN_ENV_INTERFACE_HXX
#define JARALYN_ENV_INTERFACE_HXX

// not sure how to get around the problem of not having
// virtual function templates so we need to access the
// entt::dispatcher directly

/**
 * @brief Observer interface for {@link j_env_event}s
 */
class j_env_event_listener {
public:
    /**
     * @brief Register an instance to the env event dispatcher
     *
     * You can subscribe to multiple env events.
     */
    virtual void attach(entt::dispatcher& dispatcher) = 0;
};

/**
 * @brief An abstraction over raw SDL2 events
 */
struct j_env_event {
};

/**
 * @brief Triggered on SDL_QUIT when closing the window
 *
 * This may occur when Alt+F4 is pressed or the X window
 * button is closed.
 */
struct j_quit_event : public j_env_event {
};

/**
 * @brief Triggered on SDL_WINDOWEVENT_RESIZED as the user resizes the window.
 */
struct j_resize_event : public j_env_event {
    j_size<uint32_t> size;

    explicit j_resize_event(j_size<uint32_t> s) : size { s } {}
};

/**
 * @brief Triggered on SDL_MOUSEMOTION as the user moves the mouse inside the window.
 */
struct j_mouse_move_event : public j_env_event {
    j_vec2<int32_t> position { 0, 0 };

    explicit j_mouse_move_event(j_vec2<int32_t> p) : position { p } {}
};

/**
 * @brief Triggered on SDL_MOUSEBUTTONDOWN (only when inside the window)
 */
struct j_mouse_down_event : public j_env_event {
    j_mouse_button button;

    explicit j_mouse_down_event(j_mouse_button b) : button { b } {}
};

/**
 * @brief Triggered on SDL_MOUSEBUTTONUP (only when inside the window)
 */
struct j_mouse_up_event : public j_env_event {
    j_mouse_button button;

    explicit j_mouse_up_event(j_mouse_button b) : button { b } {}
};

/**
 * @brief Triggered on SDL_KEYDOWN (only when window is focused)
 */
struct j_key_down_event : public j_env_event {
    SDL_Keycode key;

    explicit j_key_down_event(SDL_Keycode k) : key { k } {}
};

/**
 * @brief Triggered on SDL_KEYUP (only when window is focused)
 */
struct j_key_up_event : public j_env_event {
    SDL_Keycode key;

    explicit j_key_up_event(SDL_Keycode k) : key { k } {}
};

#endif