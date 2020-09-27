#ifndef JARALYN_STATE_HXX
#define JARALYN_STATE_HXX

enum class j_state_id {
    world,
    inventory,
    status,
};

#include "../gfx/display.hxx"

class j_state {
public:
    virtual ~j_state() = default;

    virtual void on_enter() {
    }

    virtual void on_update() {
    }

    virtual void on_render(j_display& display) {

    }

    virtual void on_leave() {
    }
};

#endif
