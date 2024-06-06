#ifndef NOEDEN_ACTION_EVENT_HXX
#define NOEDEN_ACTION_EVENT_HXX

#include "framework/event_type.hxx"

class Action;

struct ActionQueueProcessed {
    static constexpr EventType event_type = EventType::ActionQueueProcessed;
};

struct PlayerActionCommitted {
    static constexpr EventType event_type = EventType::PlayerActionCommitted;

    explicit constexpr PlayerActionCommitted(Action* action) : action(action) {
        assert(this->action);
    }

    Action* action = nullptr;
};

#endif
