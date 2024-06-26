#ifndef NOEDEN_ENTITY_EVENT_HXX
#define NOEDEN_ENTITY_EVENT_HXX

#include "framework/event_type.hxx"

class Entity;

struct PlayerMovedEvent : public Event<EventType::PlayerMoved> {
    WorldPos position;

    explicit PlayerMovedEvent(const WorldPos& position) : position(position) {}
};

struct EntityCreatedEvent : public Event<EventType::EntityCreated> {
    Entity* entity = nullptr;

    explicit EntityCreatedEvent(Entity* entity) : entity(entity) {
        assert(entity);
    }
};

#endif
