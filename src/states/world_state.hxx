#ifndef JARALYN_WORLD_STATE_HXX
#define JARALYN_WORLD_STATE_HXX

#include "../game.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"
#include "../event/player_event.hxx"
#include "state.hxx"

class j_world_state : public j_state {
private:
    entt::entity dwarf_;
    entt::entity troll_;
public:
    virtual void on_enter() override;

    virtual void on_update() override;

    virtual void on_leave() override;
};

#endif