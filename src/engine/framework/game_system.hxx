#ifndef JARALYN_GAME_SYSTEM_HXX
#define JARALYN_GAME_SYSTEM_HXX

#include "task.hxx"
#include "system_scene.hxx"

class game_system {
protected:
    system_scene scene_;

    std::vector<std::unique_ptr<task>> tasks_;
public:
    std::vector<std::unique_ptr<task>>& submit_tasks();
    virtual void attach(entt::dispatcher& dispatcher);

    /**
     * @brief A globally unique identifier across all game systems
     */
    virtual system_id_t id() const noexcept = 0;

    const system_scene& scene() const noexcept;

    virtual ~game_system() = default;
};

using system_map_t = std::map<system_id_t, std::unique_ptr<game_system>>;

#endif
