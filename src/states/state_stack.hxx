#ifndef JARALYN_STATE_STACK_HXX
#define JARALYN_STATE_STACK_HXX

#include "state_writer.hxx"
#include "world_state.hxx"

class j_state_stack : public j_state_writer {
private:
    std::vector<std::unique_ptr<j_state>> states_;

    j_state* current_ { nullptr };
public:
    void push(j_state_id state_id) override;
    void pop() override;

    void update();
};

#endif
