#ifndef JARALYN_PLATFORM_EVENT_HXX
#define JARALYN_PLATFORM_EVENT_HXX

#include "../game/config.hxx"

/**
 * @brief Triggered on SDL_QUIT when closing the window
 *
 * This may occur when Alt+F4 is pressed or the X window
 * button is closed.
 */
struct QuitEvent {
};

/**
 * @brief Triggered as the user resizes the window.
 */
struct ResizeEvent {
    Vec2<u32> size;
};

/**
 * @brief Triggered every time the configuration updates (usually from within lua)
 */
struct ConfigUpdatedEvent {
    Config prev;
    Config next;

    ConfigUpdatedEvent(Config cfg_prev, Config cfg_next) :
        prev(std::move(cfg_prev)),
        next(std::move(cfg_next)) {
    }
};

#endif
