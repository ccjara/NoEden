#ifndef JARALYN_SYSTEM_HXX
#define JARALYN_SYSTEM_HXX

#include "system_interface.hxx"
#include "event/event_queue.hxx"

class j_systems;

/**
 * @brief Base class of all game systems
 */
template<typename t>
class j_system : public j_system_interface {
    friend class j_systems;
protected:
    std::unique_ptr<j_event_queue> events_;
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
