#ifndef NOEDEN_WORLD_EVENT_HXX
#define NOEDEN_WORLD_EVENT_HXX

#include "framework/engine_event_type.hxx"

struct WorldReadyEvent {
    static constexpr EngineEventType event_type = EngineEventType::WorldReady;
};

struct WorldUpdatedPreEvent {
    static constexpr EngineEventType event_type = EngineEventType::WorldUpdatedPre;
};

struct WorldUpdatedPostEvent {
    static constexpr EngineEventType event_type = EngineEventType::WorldUpdatedPost;
};

#endif
