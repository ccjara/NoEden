#ifndef REALM_EVENT_HXX
#define REALM_EVENT_HXX

#include "framework/event_type.hxx"

class Realm;

/**
 * @brief Triggered as the user resizes the window.
 */
struct RealmLoadedEvent : Event<EventType::RealmLoaded> {
    Realm* realm = nullptr;

    explicit RealmLoadedEvent(Realm* realm) : realm(realm) { assert(realm); }
};

#endif
