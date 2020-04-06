#ifndef JARALYN_GAME_SYSTEM_HXX
#define JARALYN_GAME_SYSTEM_HXX

#include "task.hxx"
#include "system_scene.hxx"
#include "../managers/state/change_controller.hxx"

class game_system {
protected:
    std::unique_ptr<system_scene> scene_;

    std::vector<std::unique_ptr<task>> tasks_;
public:
    std::vector<std::unique_ptr<task>>& submit_tasks();

    virtual void attach_platform(entt::dispatcher& dispatcher);

    virtual void attach(change_controller& controller);

    virtual void on_load();

    // test run
    virtual void on_systems_loaded();

    /**
     * @brief A globally unique identifier across all game systems
     */
    virtual system_id_t id() const noexcept = 0;

    const system_scene& scene() const noexcept;

    virtual ~game_system() = default;
};

using system_map_t = std::map<system_id_t, std::unique_ptr<game_system>>;

#endif
