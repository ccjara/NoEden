#ifndef JARALYN_GAME_SYSTEM_HXX
#define JARALYN_GAME_SYSTEM_HXX

#include "task.hxx"
#include "../managers/manager_provider.hxx"

using system_id_t = int32_t;
class game_system {
protected:
    std::vector<std::unique_ptr<task>> tasks_;
public:
    std::vector<std::unique_ptr<task>>& submit_tasks();
    virtual void attach(entt::dispatcher& dispatcher);

    /**
     * @brief A globally unique identifier across all game systems
     */
    virtual system_id_t id() const noexcept = 0;

    virtual ~game_system() = default;
};

#endif
