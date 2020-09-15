#ifndef JARALYN_STATE_STACK_HXX
#define JARALYN_STATE_STACK_HXX

#include "world_state.hxx"

class j_state_stack {
private:
    std::vector<std::unique_ptr<j_state>> states_;

    j_state* current_ { nullptr };
public:
    void push(j_state_id state_id);
    void pop();

    void update();
};

#endif
