#ifndef JARALYN_ENV_INTERFACE_HXX
#define JARALYN_ENV_INTERFACE_HXX

// not sure how to get around the problem of not having
// virtual function templates so we need to access the
// entt::dispatcher directly

class j_env_event_listener {
public:
    virtual void attach(entt::dispatcher& dispatcher) = 0;
};

struct j_env_event {
};

struct j_quit_event : public j_env_event {
};

struct j_resize_event : public j_env_event {
    j_size<uint32_t> size;

    explicit j_resize_event(j_size<uint32_t> s) : size { s } {}
};

struct j_mouse_move_event : public j_env_event {
    j_vec2<int32_t> position { 0, 0 };

    explicit j_mouse_move_event(j_vec2<int32_t> p) : position { p } {}
};

struct j_mouse_down_event : public j_env_event {
    j_mouse_button button;

    explicit j_mouse_down_event(j_mouse_button b) : button { b } {}
};

struct j_mouse_up_event : public j_env_event {
    j_mouse_button button;

    explicit j_mouse_up_event(j_mouse_button b) : button { b } {}
};

struct j_key_down_event : public j_env_event {
    SDL_Keycode key;

    explicit j_key_down_event(SDL_Keycode k) : key { k } {}
};

struct j_key_up_event : public j_env_event {
    SDL_Keycode key;

    explicit j_key_up_event(SDL_Keycode k) : key { k } {}
};

#endif