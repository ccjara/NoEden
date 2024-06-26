#ifndef NOEDEN_ACTION_EVENT_HXX
#define NOEDEN_ACTION_EVENT_HXX

#include "framework/event_type.hxx"

class Action;

struct ActionQueueProcessed : public Event<EventType::ActionQueueProcessed> {
};

struct PlayerActionCommitted : public Event<EventType::PlayerActionCommitted> {
    explicit PlayerActionCommitted(Action* action) : action(action) {
        assert(this->action);
    }

    Action* action = nullptr;
};

#endif
