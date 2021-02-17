#ifndef JARALYN_SCRIPT_EVENT_HXX
#define JARALYN_SCRIPT_EVENT_HXX

class j_script;

/**
 * @brief Triggered after a script loaded
 */
struct j_script_loaded_event {
    j_script* script { nullptr };

    constexpr j_script_loaded_event(j_script* script) : script { script } {
        assert(this->script);
    }
};

/**
 * @brief Triggered before a script unloads
 */
struct j_script_before_unload_event {
    j_script* script { nullptr };

    constexpr j_script_before_unload_event(j_script* script) : script { script } {
        assert(this->script);
    }
};

/**
 * @brief Triggered after a script unloaded
 */
struct j_script_unloaded_event {
    j_script* script { nullptr };

    constexpr j_script_unloaded_event(j_script* script) : script { script } {
        assert(this->script);
    }
};

#endif
