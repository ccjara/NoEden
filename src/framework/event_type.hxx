#ifndef NOEDEN_EVENT_TYPE_HXX
#define NOEDEN_EVENT_TYPE_HXX

enum class EventType {
    None,
    ConfigUpdated,
    MouseMove,
    MouseDown,
    MouseUp,
    KeyDown,
    KeyUp,
    Resize,
    DisplayResized,
    EntityCreated,
    ScriptReset,
    ScriptLoaded,
    ScriptEnvSetup,
    ActionQueueProcessed,
    PlayerActionCommitted,
    PlayerMoved,
    WorldReady,
    WorldUpdatedPre,
    WorldUpdatedPost,
};

#endif
