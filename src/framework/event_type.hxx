#ifndef NOEDEN_EVENT_TYPE_HXX
#define NOEDEN_EVENT_TYPE_HXX

enum class EventType {
    None,
    ConfigUpdated,
    MouseMove,
    MouseButtonDown,
    MouseButtonUp,
    KeyDown,
    KeyUp,
    WindowResized,
    RealmLoaded,
    DisplayResized,
    EntityCreated,
    ScriptReset,
    ScriptLoaded,
    ScriptUnloadedPre,
    ScriptEnvSetup,
    ActionQueueProcessed,
    PlayerActionCommitted,
    PlayerMoved,
    WorldReady,
    WorldUpdatedPre,
    WorldUpdatedPost,
};

#endif
