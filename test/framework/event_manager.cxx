#include <catch2/catch.hpp>


enum class EngineEventType {
    Start,
    Stop,
};

struct EngineStartEvent : public Event<EngineEventType::Start> {
    i32 value;

    explicit EngineStartEvent(i32 value) : value(value) {
    }
};

struct EngineStopEvent : public Event<EngineEventType::Stop> {
    bool value;

    explicit EngineStopEvent(bool value) : value(value) {
    }
};

TEST_CASE("EventManager calls all subscribed handlers", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    auto subOne = events.on<EngineStartEvent>([&handlerOneCalled](const EngineStartEvent& e) -> EventResult {
        handlerOneCalled = true;
        REQUIRE(e.value == 42);
        return EventResult::Continue;
    });

    auto subTwo = events.on<EngineStartEvent>([&handlerTwoCalled](const EngineStartEvent& e) -> EventResult {
        handlerTwoCalled = true;
        REQUIRE(e.value == 42);
        return EventResult::Continue;
    });

    events.trigger<EngineStartEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE(handlerTwoCalled);
}

TEST_CASE("EventManager calls only matching handlers", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    auto subOne = events.on<EngineStartEvent>([&handlerOneCalled](const EngineStartEvent& e) -> EventResult {
        handlerOneCalled = true;
        return EventResult::Continue;
    });

    auto subTwo = events.on<EngineStopEvent>([&handlerTwoCalled](const EngineStopEvent& e) -> EventResult {
        handlerTwoCalled = true;
        return EventResult::Continue;
    });

    events.trigger<EngineStartEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE_FALSE(handlerTwoCalled);
}

TEST_CASE("EventManager stops event propagation if event result is halt", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    auto subOne = events.on<EngineStartEvent>([&handlerOneCalled](const EngineStartEvent& e) -> EventResult {
        handlerOneCalled = true;
        return EventResult::Halt;
    });

    auto subTwo = events.on<EngineStartEvent>([&handlerTwoCalled](const EngineStartEvent& e) -> EventResult {
        handlerTwoCalled = true;
        return EventResult::Continue;
    });

    events.trigger<EngineStartEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE_FALSE(handlerTwoCalled);
}

TEST_CASE("EventManager calls handlers in descending order of priority", "[event][unit]") {
    EventManager<EngineEventType> events;

    bool handlerOneCalled = false;
    bool handlerTwoCalled = false;

    auto subOne = events.on<EngineStartEvent>([&handlerOneCalled, &handlerTwoCalled](const EngineStartEvent& e) -> EventResult {
        handlerOneCalled = true;
        REQUIRE(handlerTwoCalled);
        return EventResult::Continue;
    }, 100);

    auto subTwo = events.on<EngineStartEvent>([&handlerOneCalled, &handlerTwoCalled](const EngineStartEvent& e) -> EventResult {
        handlerTwoCalled = true;
        REQUIRE_FALSE(handlerOneCalled);
        return EventResult::Continue;
    }, 101);

    events.trigger<EngineStartEvent>(42);

    REQUIRE(handlerOneCalled);
    REQUIRE(handlerTwoCalled);
}

TEST_CASE("Subscription, when out of scope, removes listener", "[event][unit]") {
    EventManager<EngineEventType> events;

    {
        auto subscription = events.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        REQUIRE(events.handler_count(EngineEventType::Start) == 1);
    }

    REQUIRE(events.handler_count(EngineEventType::Start) == 0);
}

TEST_CASE("Subscription when moved into other subscription, unsubscribes from old listener", "[event][unit]") {
    EventManager<EngineEventType> events;

    auto subscription = events.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
        return EventResult::Continue;
    });

    REQUIRE(events.handler_count(EngineEventType::Start) == 1);

    {
        auto&& movedSubscription = std::move(subscription);
        REQUIRE(events.handler_count(EngineEventType::Start) == 1);
    }
}

TEST_CASE("Subscription move assignment behavior", "[Subscription]") {
    SECTION("Move assignment with different event managers: moved subscription is preserved") {
        EventManager<EngineEventType> manager1;
        EventManager<EngineEventType> manager2;

        auto sub1 = manager1.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        auto sub2 = manager2.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        const Id sub1_id = sub1.id();
        const Id sub2_id = sub2.id();

        REQUIRE(sub1_id == sub2_id); // precondition

        sub1 = std::move(sub2);

        REQUIRE(sub1.id() == sub2_id);
        REQUIRE(sub2.id() == 0); // NOLINT(*-use-after-move)

        REQUIRE(manager1.handler_count(EngineEventType::Start) == 0);
        REQUIRE(manager2.handler_count(EngineEventType::Start) == 1);
    }

    SECTION("Move assignment with different ids: moved subscription is preserved") {
        EventManager<EngineEventType> manager1;

        auto sub1 = manager1.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        auto sub2 = manager1.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        const Id sub1_id = sub1.id();
        const Id sub2_id = sub2.id();

        REQUIRE(sub1_id != sub2_id); // precondition

        sub1 = std::move(sub2);

        REQUIRE(sub1.id() == sub2_id);
        REQUIRE(sub2.id() == 0); // NOLINT(*-use-after-move)

        REQUIRE(manager1.handler_count(EngineEventType::Start) == 1);
    }

    SECTION("Move assignment with same event manager and same id, no unsubscribe") {
        EventManager<EngineEventType> manager1;

        auto sub1 = manager1.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        const Id sub1_id = sub1.id();

        Subscription<EngineStartEvent>&& same_sub = std::move(sub1);

        REQUIRE(same_sub.id() == sub1_id);
        REQUIRE(manager1.handler_count(EngineEventType::Start) == 1);
    }
}