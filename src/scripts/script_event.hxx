#ifndef JARALYN_SCRIPT_EVENT_HXX
#define JARALYN_SCRIPT_EVENT_HXX

class Script;

/**
 * @brief Triggered prior to resetting all scripts
 */
struct ScriptResetEvent {
};

/**
 * @brief Triggered after a script loaded
 */
struct ScriptLoadedEvent {
    Script* script { nullptr };

    explicit constexpr ScriptLoadedEvent(Script* script) : script { script } {
        assert(this->script);
    }
};

#endif
