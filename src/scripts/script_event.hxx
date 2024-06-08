#ifndef NOEDEN_SCRIPT_EVENT_HXX
#define NOEDEN_SCRIPT_EVENT_HXX

#include "framework/engine_event_type.hxx"

class Script;

/**
 * @brief Triggered prior to resetting all scripts
 */
struct ScriptResetEvent : public Event<EngineEventType::ScriptReset> {
};

/**
 * @brief Triggered after a script has been loaded but before its environment is set up
 */
struct ScriptLoadedEvent : public Event<EngineEventType::ScriptLoaded> {
    Script* script = nullptr;

    explicit ScriptLoadedEvent(Script* script) : script(script) {
        assert(this->script);
    }
};

/**
 * @brief Triggered after a loaded script had its environment set up
 */
struct ScriptEnvSetupEvent : public Event<EngineEventType::ScriptEnvSetup> {
    Script* script = nullptr;

    explicit ScriptEnvSetupEvent(Script* script) : script(script) {
        assert(this->script);
    }
};

#endif
