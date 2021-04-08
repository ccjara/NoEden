#ifndef JARALYN_SCRIPT_EVENT_HXX
#define JARALYN_SCRIPT_EVENT_HXX

class j_script;

/**
 * @brief Triggered prior to resetting all scripts
 */
struct j_script_reset_event {
};

/**
 * @brief Triggered after a script loaded
 */
struct j_script_loaded_event {
    j_script* script { nullptr };

    explicit constexpr j_script_loaded_event(j_script* script) : script { script } {
        assert(this->script);
    }
};

#endif
