#ifndef JARALYN_SYSTEMS_HXX
#define JARALYN_SYSTEMS_HXX

#include "system_interface.hxx"

/**
 * @brief Managing class of all game systems
 */
class j_systems {
private:
    entt::dispatcher* dispatcher_ { nullptr };

    std::vector<std::unique_ptr<j_system_interface>> systems_;
    std::unordered_map<j_static_id_t, j_system_interface*> systems_by_id_;
public:
    j_systems(entt::dispatcher* dispatcher);
    ~j_systems();

    j_systems(const j_systems&) = delete;
    j_systems& operator=(const j_systems&) = delete;

    j_systems(j_systems&&) = delete;
    j_systems& operator=(j_systems&&) = delete;

    /**
     * @brief Unloads all game systems in reverse order of emplacement
     */
    void unload();

    template<typename sys>
    sys* get() {
        const auto id { sys::static_id };
        const auto iter { systems_by_id_.find(id) };
        if (iter == systems_by_id_.end()) {
            return nullptr;
        }
        return static_cast<sys*> (iter->second);
    }

    j_system_interface* get(j_static_id_t id) {
        const auto iter { systems_by_id_.find(id) };
        if (iter == systems_by_id_.end()) {
            return nullptr;
        }
        return iter->second;
    }

    /**
     * @brief Adds a new system to the system container
     *
     * Arguments provided are forwarded to the system constructor.
     */
    template<typename sys, typename... args>
    sys* emplace(args&&... sys_args) {
        const auto id { sys::static_id };
        const auto iter { systems_by_id_.find(id) };
        if (iter != systems_by_id_.end()) {
            LOG(WARNING) << "Tried to emplace the same system "
                         << id
                         << " twice, skipping and returning the existing one";
            return static_cast<sys*> (iter->second);
        }
        sys* system { new sys(std::forward<args>(sys_args)...) };
        systems_.emplace_back(system);
        systems_by_id_.try_emplace(id, system);
        system->systems_ = this;
        system->dispatcher_ = dispatcher_;
        system->events_ = std::make_unique<j_event_queue>(dispatcher_);
        system->on_load();

        return system;
    }

    void update(float_t delta_time);
};

#endif
