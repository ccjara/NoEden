#ifndef NOEDEN_GFX_EVENT_HXX
#define NOEDEN_GFX_EVENT_HXX

#include "framework/engine_event_type.hxx"

/**
 * @brief Triggered after the display has been resized
 *
 * The display is resized in the gfx system just after the low level event
 * ResizeEvent has been handled there (for example due to a window resize).
 */
struct DisplayResizedEvent : public Event<EngineEventType::DisplayResized> {
    /**
     * @brief The new display size in cell units
     */
    Vec2<u32> size;

    explicit DisplayResizedEvent(const Vec2<u32>& size) : size(size) {
    }
};

#endif
