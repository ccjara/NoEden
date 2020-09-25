#ifndef JARALYN_HUD_SYSTEM_HXX
#define JARALYN_HUD_SYSTEM_HXX

#include "../game.hxx"
#include "journal.hxx"
#include "../components/components.hxx"

class j_hud_system : public j_system<j_hud_system> {
private:
    j_journal journal_;

    void task_journal_item_pickup(const j_gathering_completed_event& e);
public:
    void on_load() override;

    void update(uint32_t delta_time) override;

    /**
     * @brief Returns all journal entries which should be rendered to display
     *
     * The return type will change later as soon as "render jobs" are implemented
     */
    const std::vector<std::string>& journal_entries() const;
};

#endif
