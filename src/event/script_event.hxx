#ifndef JARALYN_SCRIPT_EVENT_HXX
#define JARALYN_SCRIPT_EVENT_HXX

class j_script;

/**
 * @brief Triggered when a script loads or reloads
 */
struct j_script_loaded_event {
    j_script* script { nullptr };
    bool reloaded { false };

    constexpr j_script_loaded_event(j_script* const script, bool reloaded) :
        script { script }, reloaded { reloaded } {
        assert(this->script);
    }
};

#endif
