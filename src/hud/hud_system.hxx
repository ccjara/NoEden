#ifndef JARALYN_HUD_SYSTEM_HXX
#define JARALYN_HUD_SYSTEM_HXX

#include "../game.hxx"
#include "../system.hxx"
#include "../event/event.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"

class j_hud_system : public j_system<j_hud_system> {
private:
    entt::entity status_text_;
    entt::observer item_observer_;

    void on_player_movement(const j_player_moved_event& e);
public:
    void on_load() override;

    void update(uint32_t delta_time) override;
};

#endif
