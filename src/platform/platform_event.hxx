#ifndef NOEDEN_PLATFORM_EVENT_HXX
#define NOEDEN_PLATFORM_EVENT_HXX

#include "framework/event_type.hxx"

/**
 * @brief Triggered as the user resizes the window.
 */
struct ResizeEvent : Event<EventType::Resize> {
    Vec2<i32> size = { 0, 0 };

    explicit ResizeEvent(Vec2<i32> size) : size(size) {}
};

#endif
