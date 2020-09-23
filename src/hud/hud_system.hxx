#ifndef JARALYN_HUD_SYSTEM_HXX
#define JARALYN_HUD_SYSTEM_HXX

#include "../game.hxx"
#include "../system.hxx"
#include "../event/player_event.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"
#include "journal.hxx"

class j_hud_system : public j_system<j_hud_system> {
private:
    j_journal journal_;
    std::string status_;

    entt::observer item_observer_;

    void on_player_movement(const j_player_moved_event& e);
public:
    void on_load() override;

    void update(uint32_t delta_time) override;

    /**
     * @brief Returns all journal entries which should be rendered to display
     *
     * The return type will change later as soon as "render jobs" are implemented
     */
    const std::vector<std::string>& journal_entries() const;

    /**
     * @brief Returns the status text - not sure where we are going with this
     */
    const std::string& status() const;
};

#endif
