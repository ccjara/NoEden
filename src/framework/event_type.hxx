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
    ScriptReset,
    ScriptLoaded,
    ActionQueueProcessed,
    PlayerActionCommitted,
    WorldReady,
    WorldUpdatedPre,
    WorldUpdatedPost,
};

#endif
