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


TEST_CASE("EventManager::clear() removes all handlers", "[event][unit]") {
    EventManager<EngineEventType> events;

    auto subOne = events.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
        return EventResult::Continue;
    });

    auto subTwo = events.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
        return EventResult::Continue;
    });

    REQUIRE(events.handler_count(EngineEventType::Start) == 2);

    events.clear();

    REQUIRE(events.handler_count(EngineEventType::Start) == 0);
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
    SECTION("Move assignment to inactive subscription: moved subscription is preserved") {
        EventManager<EngineEventType> manager;
        Subscription<EngineStartEvent> sub1;

        auto sub2 = manager.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        const Id sub2_id = sub2.id();

        sub1 = std::move(sub2);

        REQUIRE(sub1.id() == sub2_id);
        REQUIRE(sub1.active() == true);
        REQUIRE(sub1.event_manager() == &manager);
        REQUIRE(sub2.id() == 0); // NOLINT(*-use-after-move)
        REQUIRE(sub2.event_manager() == nullptr); // NOLINT(*-use-after-move)
        REQUIRE_FALSE(sub2.active()); // NOLINT(*-use-after-move)
        REQUIRE(manager.handler_count(EngineEventType::Start) == 1);
    }

    SECTION("Move assignment to active subscription: unsubscribes old subscription") {
        EventManager<EngineEventType> manager;

        auto sub1 = manager.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        auto sub2 = manager.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        const Id sub1_id = sub1.id();
        const Id sub2_id = sub2.id();

        REQUIRE(sub1_id != sub2_id); // precondition
        REQUIRE(manager.handler_count(EngineEventType::Start) == 2); // precondition

        sub1 = std::move(sub2);

        REQUIRE(sub1.id() == sub2_id);
        REQUIRE(sub1.active() == true);
        REQUIRE(sub1.event_manager() == &manager);
        REQUIRE(sub2.id() == 0); // NOLINT(*-use-after-move)
        REQUIRE(sub2.event_manager() == nullptr); // NOLINT(*-use-after-move)
        REQUIRE_FALSE(sub2.active()); // NOLINT(*-use-after-move)
        REQUIRE(manager.handler_count(EngineEventType::Start) == 1);
    }
}

TEST_CASE("Subscription state", "[Subscription]") {
    SECTION("Default constructed is inactive") {
        Subscription<EngineStartEvent> subscription;
        REQUIRE_FALSE(subscription.active());
    }

    SECTION("Constructed with null event manager is inactive") {
        Subscription<EngineStartEvent> subscription(nullptr, 42);
        REQUIRE_FALSE(subscription.active());
    }

    SECTION("Constructed with zero subscription id is inactive") {
        EventManager<EngineEventType> manager;
        Subscription<EngineStartEvent> subscription(&manager, 0);
        REQUIRE_FALSE(subscription.active());
    }

    SECTION("Constructed with valid event manager and id is active") {
        EventManager<EngineEventType> manager;
        Subscription<EngineStartEvent> subscription(&manager, 42);
        REQUIRE(subscription.active());
    }
}

TEST_CASE("Subscription::unsubscribe()", "[Subscription]") {
    SECTION("When inactive, does nothing") {
        Subscription<EngineStartEvent> subscription;
        subscription.unsubscribe();
        REQUIRE_FALSE(subscription.active());
    }

    SECTION("When active, unsubscribes from the event manager") {
        EventManager<EngineEventType> manager;
        auto sub = manager.on<EngineStartEvent>([](const EngineStartEvent& e) -> EventResult {
            return EventResult::Continue;
        });

        REQUIRE(sub.active());
        REQUIRE(sub.event_manager() == &manager);
        REQUIRE(sub.id() != 0);

        sub.unsubscribe();

        REQUIRE_FALSE(sub.active());
        REQUIRE(sub.event_manager() == nullptr);
        REQUIRE(sub.id() == 0);
    }
}