#ifndef NOEDEN_PLATFORM_EVENT_HXX
#define NOEDEN_PLATFORM_EVENT_HXX

#include "framework/engine_event_type.hxx"
#include "config/config.hxx"

/**
 * @brief Triggered as the user resizes the window.
 */
struct ResizeEvent : public Event<EngineEventType::Resize> {
    Vec2<i32> size;

    constexpr ResizeEvent(const Vec2<i32> size) : size(size) {}
};

#endif
