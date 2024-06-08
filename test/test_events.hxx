#ifndef NOEDENTEST_TEST_EVENTS_HXX
#define NOEDENTEST_TEST_EVENTS_HXX

#include "framework/event_manager.hxx"
#include "framework/events.hxx"

enum class GameEventType;
enum class EngineEventType;

struct TestEvents : public Events {
    TestEvents() : Events(&game_, &engine_) {} // NOLINT(*-use-equals-default)
private:
    EventManager<GameEventType> game_;
    EventManager<EngineEventType> engine_;
};

#endif
