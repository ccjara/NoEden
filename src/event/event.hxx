#ifndef JARALYN_EVENT_HXX
#define JARALYN_EVENT_HXX

#include "../env/root_config.hxx"
#include "../env/window.hxx"
#include "../scripts/script.hxx"

class j_event_listener {
public:
    /**
     * @brief Register an instance to the event dispatcher
     *
     * You can subscribe to multiple events.
     */
    virtual void attach(entt::dispatcher& dispatcher) noexcept = 0;

    virtual ~j_event_listener() = default;
};

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
    j_size<uint32_t> size;

    explicit j_resize_event(j_size<uint32_t> s) : size { s } {}
};

/**
 * @brief Triggered on SDL_MOUSEMOTION as the user moves the mouse inside the window.
 */
struct j_mouse_move_event {
    j_vec2<int32_t> position { 0, 0 };

    explicit j_mouse_move_event(j_vec2<int32_t> p) : position { p } {}
};

/**
 * @brief Triggered on SDL_MOUSEBUTTONDOWN (only when inside the window)
 */
struct j_mouse_down_event {
    j_mouse_button button;

    explicit j_mouse_down_event(j_mouse_button b) : button { b } {}
};

/**
 * @brief Triggered on SDL_MOUSEBUTTONUP (only when inside the window)
 */
struct j_mouse_up_event {
    j_mouse_button button;

    explicit j_mouse_up_event(j_mouse_button b) : button { b } {}
};

/**
 * @brief Triggered on SDL_KEYDOWN (only when window is focused)
 */
struct j_key_down_event {
    SDL_Keycode key;

    explicit j_key_down_event(SDL_Keycode k) : key { k } {}
};

/**
 * @brief Triggered on SDL_KEYUP (only when window is focused)
 */
struct j_key_up_event {
    SDL_Keycode key;

    explicit j_key_up_event(SDL_Keycode k) : key { k } {}
};

/**
 * @brief Triggered after the (SDL managed) game window has been created
 */
struct j_window_created_event {
    j_window* window { nullptr };
    explicit constexpr j_window_created_event(j_window* const window) : window { window } {
        assert(this->window);
    }
};

/**
 * @brief Triggered when a script loads or reloads
 */
struct j_script_loaded_event {
    j_script* script { nullptr };
    bool reloaded { false };

    constexpr j_script_loaded_event(j_script* const script, bool reloaded) :
        script { script }, reloaded { reloaded } {
        assert(this->script);
    }
};

/**
 * @brief Triggered every time the system script is (re)loaded
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

