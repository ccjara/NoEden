#ifndef JARALYN_EVENT_HXX
#define JARALYN_EVENT_HXX

#include "../env/root_config.hxx"
#include "../components/inventory.hxx"

class j_display;
class j_script;
class j_window;

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

    explicit j_resize_event(j_vec2<uint32_t> s) : size { s } {}
};

/**
 * @brief Triggered on SDL_MOUSEMOTION as the user moves the mouse inside the window.
 */
struct j_mouse_move_event {
    j_vec2<int32_t> position;

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

/**
 * @brief Triggered when a scene renders
 *
 * May get triggered multiple times during one render tick if the scene is not
 * opaque, i.e not render-blocking.
 *
 * Does not trigger for the null scene.
 */
struct j_scene_render_event {
    j_display* display { nullptr };
    j_scene_type scene_type { j_scene_type::null };

    constexpr j_scene_render_event(j_scene_type scene_type, j_display* const display) :
        display { display }, scene_type { scene_type } {
        assert(this->display);
    }
};

/**
 * @brief Triggered when an item gets added to any inventory
 */
struct j_inventory_item_added_event {
    j_item* item { nullptr };

    constexpr j_inventory_item_added_event(j_item* const item) :
        item { item } {
        assert(item);
    }
};

#endif

