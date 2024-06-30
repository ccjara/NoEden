#include "realm/realm_manager.hxx"
#include "realm/realm_event.hxx"
#include <catch2/catch_test_macros.hpp>

enum class RealmType { Test, Test2 };

static constexpr std::string TestStr = "TEST";
static constexpr std::string Test2Str = "TEST2";

RealmType realm_type_from_string(const std::string& str) {
    if (str == "TEST2") {
        return RealmType::Test2;
    }
    return RealmType::Test;
}
const std::string& realm_type_to_string(RealmType type) { return type == RealmType::Test2 ? Test2Str : TestStr; }

TEST_CASE("RealmManager::add_realm() initialization", "[realm][unit]") {
    EventManager events;
    ServiceLocator services;

    class TestRealmTrue : public Realm {
    public:
        TestRealmTrue() : Realm(RealmType::Test) {}
        bool initialize() override { return true; }
    };
    class TestRealmFalse : public Realm {
    public:
        TestRealmFalse() : Realm(RealmType::Test) {}
        bool initialize() override { return false; }
    };

    SECTION("Returns true when Realm::initialize() returns true") {
        RealmManager manager(&services, &events);
        REQUIRE(manager.add_realm(std::make_unique<TestRealmTrue>()));
    }

    SECTION("Stores realm on successful initialization") {
        RealmManager manager(&services, &events);
        bool _ = manager.add_realm(std::make_unique<TestRealmTrue>());
        auto* realm = static_cast<TestRealmTrue*>(manager.realms().at(RealmType::Test).get());
        REQUIRE(realm != nullptr);
    }

    SECTION("Returns false when Realm::initialize() returns false") {
        RealmManager manager(&services, &events);
        REQUIRE_FALSE(manager.add_realm(std::make_unique<TestRealmFalse>()));
    }

    SECTION("Discards realm on failed initialization") {
        RealmManager manager(&services, &events);
        bool _ = manager.add_realm(std::make_unique<TestRealmFalse>());
        REQUIRE_FALSE(manager.realms().contains(RealmType::Test));
    }

    SECTION("Discards realm if realm type already exists") {
        RealmManager manager(&services, &events);

        bool _ = manager.add_realm(std::make_unique<TestRealmTrue>());
        auto* realm = static_cast<TestRealmTrue*>(manager.realms().at(RealmType::Test).get());
        REQUIRE(realm != nullptr);
        REQUIRE_FALSE(manager.add_realm(std::make_unique<TestRealmTrue>()));
        REQUIRE(manager.realms().at(RealmType::Test).get() == realm);
    }
}

TEST_CASE("RealmManager::add_realm() injects events_ and services_ before initialize is called", "[realm][unit]") {
    EventManager events;
    ServiceLocator services;

    class TestRealm : public Realm {
    public:
        TestRealm(ServiceLocator* expected_services, EventManager* expected_events) : Realm(RealmType::Test) {
            this->expected_events = expected_events;
            this->expected_services = expected_services;
        }

        EventManager* expected_events;
        ServiceLocator* expected_services;
        bool initialize_called = false;

        bool initialize() override {
            REQUIRE(events_ == expected_events);
            REQUIRE(services_ == expected_services);
            initialize_called = true;
            return true;
        }
    };

    RealmManager manager(&services, &events);

    bool _ = manager.add_realm(std::make_unique<TestRealm>(&services, &events));

    auto* realm = static_cast<TestRealm*>(manager.realms().at(RealmType::Test).get());

    REQUIRE(realm->initialize_called);
    REQUIRE(realm->expected_events != nullptr);
    REQUIRE(realm->expected_services != nullptr);
}

TEST_CASE("RealmManager::switch_realm()", "[realm][unit]") {
    EventManager events;
    ServiceLocator services;

    class TestRealm : public Realm {
    public:
        TestRealm(RealmType type) : Realm(type) {}

        void unload() override { unload_called = true; }
        void load() override { load_called = true; }
        bool load_called = false;
        bool unload_called = false;
    };

    RealmManager manager(&services, &events);

    auto tr1 = std::make_unique<TestRealm>(RealmType::Test);
    auto* tr1_ptr = tr1.get();
    REQUIRE(manager.add_realm(std::move(tr1)));

    SECTION("Returns true and switches realm if it exists") {
        REQUIRE(manager.current_realm() == nullptr);
        REQUIRE(manager.switch_realm(RealmType::Test));
        REQUIRE(manager.current_realm() == tr1_ptr);
    }

    SECTION("Returns false if Realm does not exist") {
        REQUIRE_FALSE(manager.switch_realm(RealmType::Test2));
    }

    SECTION("Calls load on the switched realm") {
        REQUIRE_FALSE(tr1_ptr->load_called);
        REQUIRE(manager.switch_realm(RealmType::Test));
        REQUIRE(tr1_ptr->load_called);
    }

    SECTION("Triggers RealmLoadedEvent") {
        bool event_triggered = false;
        auto sub = events.on<RealmLoadedEvent>([&](const RealmLoadedEvent& event) {
            REQUIRE(event.realm == tr1_ptr);
            event_triggered = true;
            return EventResult::Continue;
        });
        REQUIRE(manager.switch_realm(RealmType::Test));
        REQUIRE(event_triggered);
    }

    SECTION("Unloads current realm") {
        auto tr2 = std::make_unique<TestRealm>(RealmType::Test2);
        auto* tr2_ptr = tr2.get();
        REQUIRE(manager.add_realm(std::move(tr2)));

        REQUIRE(manager.switch_realm(RealmType::Test));
        REQUIRE(manager.switch_realm(RealmType::Test2));

        REQUIRE(tr1_ptr->load_called);
        REQUIRE(tr2_ptr->load_called);

        REQUIRE(tr1_ptr->unload_called);
        REQUIRE_FALSE(tr2_ptr->unload_called);

        REQUIRE(manager.current_realm() == tr2_ptr);
    }

    SECTION("Returns false if Realm does not exist", "[realm][unit]") {
        REQUIRE_FALSE(manager.switch_realm(RealmType::Test2));
    }
}
