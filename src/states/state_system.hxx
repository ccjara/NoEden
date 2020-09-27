#ifndef JARALYN_STATE_STACK_HXX
#define JARALYN_STATE_STACK_HXX

#include "../system.hxx"
#include "world_state.hxx"
#include "state.hxx"

class j_state_system : public j_system<j_state_system> {
private:
    std::vector<std::unique_ptr<j_state>> states_;

    j_state* current_ { nullptr };
public:
    void push(j_state_id state_id);
    void pop();

    void on_load() override;
    void update(uint32_t delta_time) override;
    void render(j_display& display);
};

#endif
