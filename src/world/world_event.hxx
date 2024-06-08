#ifndef NOEDEN_WORLD_EVENT_HXX
#define NOEDEN_WORLD_EVENT_HXX

#include "framework/engine_event_type.hxx"

struct WorldReadyEvent : public Event<EngineEventType::WorldReady> {
};

struct WorldUpdatedPreEvent : public Event<EngineEventType::WorldUpdatedPre> {
};

struct WorldUpdatedPostEvent : public Event<EngineEventType::WorldUpdatedPost> {
};

#endif
