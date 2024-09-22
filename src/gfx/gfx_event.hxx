#ifndef NOEDEN_GFX_EVENT_HXX
#define NOEDEN_GFX_EVENT_HXX

#include "framework/event_type.hxx"

/**
 * @brief Triggered after the display has been resized
 *
 * The display is resized in the gfx system just after the low level event
 * ResizeEvent has been handled there (for example due to a window resize).
 */
struct DisplayResizedEvent : Event<EventType::DisplayResized> {
    /**
     * @brief The new display size in cell units
     */
    glm::ivec2 size = {0, 0};

    explicit DisplayResizedEvent(const glm::ivec2& size) : size(size) {
    }
};

#endif
