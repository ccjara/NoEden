#ifndef JARALYN_GFX_EVENT_HXX
#define JARALYN_GFX_EVENT_HXX

/**
 * @brief Triggered after rendering the game and before swapping buffers
 */
struct j_post_render_event {
};

/**
 * @brief Triggered after the display has been resized
 *
 * The display is resized in the gfx system just after the low level event
 * j_resize_event has been handled there (for example due to a window resize).
 */
struct j_display_resized_event {
    /**
     * @brief The new display size in cell units
     */
    j_vec2<uint32_t> size;

    constexpr j_display_resized_event(j_vec2<uint32_t> size) : size(size) {
    }
};

#endif
