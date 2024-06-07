#ifndef NOEDEN_SCRIPT_EVENT_HXX
#define NOEDEN_SCRIPT_EVENT_HXX

#include "framework/event_type.hxx"

class Script;

/**
 * @brief Triggered prior to resetting all scripts
 */
struct ScriptResetEvent {
    static constexpr EventType event_type = EventType::ScriptReset;
};

/**
 * @brief Triggered after a script has been loaded but before it is run
 */
struct ScriptLoadedEvent {
    static constexpr EventType event_type = EventType::ScriptLoaded;

    Script* script = nullptr;

    explicit ScriptLoadedEvent(Script* script) : script(script) {
        assert(this->script);
    }
};

#endif
