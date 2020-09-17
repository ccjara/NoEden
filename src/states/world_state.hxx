#ifndef JARALYN_WORLD_STATE_HXX
#define JARALYN_WORLD_STATE_HXX

#include "state.hxx"
#include "../event/event.hxx"

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
