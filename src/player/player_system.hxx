#ifndef JARALYN_PLAYER_SYSTEM_HXX
#define JARALYN_PLAYER_SYSTEM_HXX

#include "../game.hxx"
#include "../system.hxx"
#include "../event/platform_event.hxx"
#include "../event/player_event.hxx"
#include "../event/inventory_event.hxx"
#include "../components/components.hxx"

class j_player_system : public j_system<j_player_system> {
private:
    j_vec2<int8_t> velocity_;

    entt::entity player_ { entt::null };

    void on_player_control(const j_player_control_event& e);

    void on_key_down(const j_key_down_event& e);

    bool gathering_ { false };
public:
    void on_load() override;

    void update(uint32_t delta_time) override;
};

#endif
