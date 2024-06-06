#ifndef NOEDEN_CALLBACK_STORE_HXX
#define NOEDEN_CALLBACK_STORE_HXX

struct CallbackInfo {
    /**
     * @brief Unique id of this callback
     */
    Id id;

    /**
     * @brief Id of the script containing the callback
     */
    Id script_id;
};

class CallbackStore {
public:
    void store(const CallbackInfo& info);

    void remove_by_callback_id(Id callback_id);

    void remove_by_script_id(Id script_id);

    void reset();
private:
    /**
     * @brief Maps callback ids to CallbackInfo structs.
     */
    std::unordered_map<Id, CallbackInfo> callbacks_;
};

#endif
