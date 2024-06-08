#include <catch2/catch.hpp>

enum class EngineEventType {
    Test,
    Test2,
};

struct TestEvent {
    constexpr static EngineEventType event_type = EngineEventType::Test;
    i32 value;
};

struct TestEvent2 {
    constexpr static EngineEventType event_type = EngineEventType::Test2;
    bool value;
};

TEST_CASE("EventManager calls all subscribed handlers", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    events.on<TestEvent>([&handlerOneCalled](const TestEvent& e) -> EventResult {
        handlerOneCalled = true;
        REQUIRE(e.value == 42);
        return EventResult::Continue;
    });

    events.on<TestEvent>([&handlerTwoCalled](const TestEvent& e) -> EventResult {
        handlerTwoCalled = true;
        REQUIRE(e.value == 42);
        return EventResult::Continue;
    });

    events.trigger<TestEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE(handlerTwoCalled);
}

TEST_CASE("EventManager calls only matching handlers", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    events.on<TestEvent>([&handlerOneCalled](const TestEvent& e) -> EventResult {
        handlerOneCalled = true;
        return EventResult::Continue;
    });

    events.on<TestEvent2>([&handlerTwoCalled](const TestEvent2& e) -> EventResult {
        handlerTwoCalled = true;
        return EventResult::Continue;
    });

    events.trigger<TestEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE_FALSE(handlerTwoCalled);
}

TEST_CASE("EventManager stops event propagation if event result is halt", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    events.on<TestEvent>([&handlerOneCalled](const TestEvent& e) -> EventResult {
        handlerOneCalled = true;
        return EventResult::Halt;
    });

    events.on<TestEvent>([&handlerTwoCalled](const TestEvent& e) -> EventResult {
        handlerTwoCalled = true;
        return EventResult::Continue;
    });

    events.trigger<TestEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE_FALSE(handlerTwoCalled);
}

TEST_CASE("EventManager calls handlers in descending order of priority", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    events.on<TestEvent>([&handlerOneCalled, &handlerTwoCalled](const TestEvent& e) -> EventResult {
        handlerOneCalled = true;
        REQUIRE(handlerTwoCalled);
        return EventResult::Continue;
    }, 100);

    events.on<TestEvent>([&handlerOneCalled, &handlerTwoCalled](const TestEvent& e) -> EventResult {
        handlerTwoCalled = true;
        REQUIRE_FALSE(handlerOneCalled);
        return EventResult::Continue;
    }, 101);

    events.trigger<TestEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE(handlerTwoCalled);
}
