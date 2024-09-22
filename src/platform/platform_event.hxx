#ifndef NOEDEN_PLATFORM_EVENT_HXX
#define NOEDEN_PLATFORM_EVENT_HXX

#include "framework/event_type.hxx"

/**
 * @brief Triggered as the user resizes the window.
 */
struct WindowResizedEvent : Event<EventType::WindowResized> {
    glm::ivec2 size = { 0, 0 };

    explicit WindowResizedEvent(glm::ivec2 size) : size(size) {}
};

#endif
