#ifndef NOEDEN_WORLD_EVENT_HXX
#define NOEDEN_WORLD_EVENT_HXX

#include "framework/event_type.hxx"

class WorldSpec;

struct WorldReadyEvent : public Event<EventType::WorldReady> {
    WorldSpec* world_spec = nullptr;

    WorldReadyEvent(WorldSpec* world_spec) : world_spec(world_spec) {
        assert(world_spec);
    }
};

struct WorldUpdatedPreEvent : public Event<EventType::WorldUpdatedPre> {
};

struct WorldUpdatedPostEvent : public Event<EventType::WorldUpdatedPost> {
};

#endif
