#ifndef JARALYN_WORLD_EVENT_HXX
#define JARALYN_WORLD_EVENT_HXX

#include "framework/event_type.hxx"

struct WorldReadyEvent {
    static constexpr EventType event_type = EventType::WorldReady;
};

struct WorldUpdatedPreEvent {
    static constexpr EventType event_type = EventType::WorldUpdatedPre;
};

struct WorldUpdatedPostEvent {
    static constexpr EventType event_type = EventType::WorldUpdatedPost;
};

#endif
