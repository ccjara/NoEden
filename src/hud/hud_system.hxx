#ifndef JARALYN_HUD_SYSTEM_HXX
#define JARALYN_HUD_SYSTEM_HXX

#include "../game.hxx"
#include "../system.hxx"
#include "../event/inventory_event.hxx"
#include "journal.hxx"
#include "../components/components.hxx"
#include "ui_tree.hxx"
#include "ui_proxy.hxx"
#include "ui_window_proxy.hxx"

class j_hud_system : public j_system<j_hud_system> {
private:
    j_journal journal_;

    void immediate_on_script_loaded(const j_script_loaded_event& e);
    void immediate_on_script_before_unload(const j_script_before_unload_event& e);

    void task_show_inventory_ui(const j_inventory_view_event& e);
    void task_journal_item_pickup(const j_gathering_completed_event& e);

    j_ui_window* inventory_window_ { nullptr };

    j_ui_tree ui_;
    j_ui_proxy ui_proxy_;
public:
    j_hud_system() : ui_proxy_(&ui_) {}

    void on_load() override;

    void update(uint32_t delta_time) override;

    /**
     * @brief Returns all journal entries which should be rendered to display
     *
     * The return type will change later as soon as "render jobs" are implemented
     */
    const std::vector<std::string>& journal_entries() const;

    j_ui_tree& ui_tree();
};

#endif
