#ifndef JARALYN_CALLBACK_STORE_HXX
#define JARALYN_CALLBACK_STORE_HXX

struct CallbackInfo {
    /**
     * @brief Unique id for this callback
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
private:
    /**
     * @brief Maps callback ids to CallbackInfo structs.
     *
     * If performance becomes a concern indexing dictionaries can be added.
     */
    std::unordered_map<Id, CallbackInfo> callbacks_;
};

#endif
