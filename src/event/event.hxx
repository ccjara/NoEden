#ifndef JARALYN_EVENT_HXX
#define JARALYN_EVENT_HXX

#include "../env/root_config.hxx"
#include "../components/item.hxx"

class j_script;

// TODO: split up

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
 * @brief Triggered when an item gets added to any inventory
 */
struct j_item_stored_event {
    entt::entity item;

    explicit constexpr j_item_stored_event(entt::entity item) : item(item) {
    }
};

/**
 * @brief Triggered as soon as the player assumes control of a unit
 */
struct j_player_control_event {
    entt::entity player;

    explicit constexpr j_player_control_event(entt::entity player) : player(player) {
    }
};

/**
 * @brief Triggered when the current controllable unit moved
 */
struct j_player_moved_event {
    entt::entity player;
    j_vec2<uint32_t> position;

    constexpr j_player_moved_event(entt::entity player, j_vec2<uint32_t> position) :
        player(player),
        position(position) {
    }
};

#endif
