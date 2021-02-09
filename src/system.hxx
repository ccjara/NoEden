#ifndef JARALYN_SYSTEM_HXX
#define JARALYN_SYSTEM_HXX

#include "system_interface.hxx"

class j_systems;

/**
 * @brief Base class of all game systems
 */
template<typename t>
class j_system : public j_system_interface {
    friend class j_systems;

    /**
     * @brief Creates a copy for incoming game events for the implementing system
     *
     * Not really sure how to get rid of this function. I tried connecting
     * to the entt::dispatcher::enqueue<event_source> function pointer but got
     * template error nightmares.
     */
    template<typename event_source>
    inline void shovel(const event_source& e) {
        queue_.enqueue(e);
    }
protected:
    /**
     * @brief Contains all transcribed events which the system is interested in
     *
     * Note: The executors must only be executed in the update method of the
     *       system.
     */
    entt::dispatcher queue_;

    /**
    * @brief Attaches to the given game event and registers an executor
    *
    * Incoming events are stored aside and can be processed during the system's
    * update phase. You can process all events, or only those of the same type.
    * Refer to the entt::dispatcher::update() documentation.
    */
    template<typename event_source, auto executor>
    void define_task() {
        t* this_ { static_cast<t*> (this) };
        // register the executor that will be called when the system updates
        queue_.sink<event_source>().connect<executor>(this_);
        // setup event transcription
        dispatcher_->sink<event_source>().connect<&j_system<t>::shovel<event_source>>(this_);
    }
public:
    static const j_static_id_t static_id;

    j_systems* systems_ { nullptr };
    entt::dispatcher* dispatcher_ { nullptr };

    virtual ~j_system() = default;

    virtual void on_load() override {
    }
    
    virtual void on_unload() override {
    }

    [[nodiscard]] inline const j_static_id_t id() const override {
        return static_id;
    }
};

// generate static identity for each CRTP'ed subclass in the system group
template<typename t>
const j_static_id_t j_system<t>::static_id = j_static_identity_provider<j_system_interface>::generate<t>();

#endif
