#ifndef NOEDEN_ACTION_EVENT_HXX
#define NOEDEN_ACTION_EVENT_HXX

#include "framework/engine_event_type.hxx"

class Action;

struct ActionQueueProcessed {
    static constexpr EngineEventType event_type = EngineEventType::ActionQueueProcessed;
};

struct PlayerActionCommitted {
    static constexpr EngineEventType event_type = EngineEventType::PlayerActionCommitted;

    explicit PlayerActionCommitted(Action* action) : action(action) {
        assert(this->action);
    }

    Action* action = nullptr;
};

#endif
