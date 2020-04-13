#ifndef JARALYN_ENV_EVENT_HXX
#define JARALYN_ENV_EVENT_HXX

struct j_env_event {
};

struct j_quit_event : public j_env_event {
};

struct j_resize_event : public j_env_event {
    j_size<uint32_t> size;

    explicit j_resize_event(j_size<uint32_t> s) : size { s } {}
};

struct j_mouse_move_event : public j_env_event {
    j_position<int32_t> position { 0, 0 };

    explicit j_mouse_move_event(j_position<int32_t> p) : position { p } {}
};

struct j_mouse_down_event : public j_env_event {
    j_mouse_button button;

    explicit j_mouse_down_event(j_mouse_button b) : button { b } {}
};

struct j_mouse_up_event : public j_env_event {
    j_mouse_button button;

    explicit j_mouse_up_event(j_mouse_button b) : button { b } {}
};

#endif
