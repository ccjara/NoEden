#include "input_system.hxx"

void j_input_system::update(uint32_t delta_time) {
    SDL_Event e;

    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_KEYDOWN, SDL_EventType::SDL_MOUSEWHEEL)) {
        switch (e.type) {
        case SDL_EventType::SDL_KEYDOWN:
            keyboard_.key_down(e.key.keysym.sym);
            dispatcher_->trigger<j_key_down_event>(e.key.keysym.sym);
            break;
        case SDL_EventType::SDL_KEYUP:
            keyboard_.key_up(e.key.keysym.sym);
            dispatcher_->trigger<j_key_up_event>(e.key.keysym.sym);
            break;
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            mouse_.mouse_down(static_cast<j_mouse_button>(e.button.button));
            dispatcher_->trigger<j_mouse_down_event>(
                static_cast<j_mouse_button>(e.button.button)
            );
            break;
        case SDL_EventType::SDL_MOUSEBUTTONUP:
            mouse_.mouse_up(static_cast<j_mouse_button>(e.button.button));
            dispatcher_->trigger<j_mouse_up_event>(
                static_cast<j_mouse_button>(e.button.button)
            );
            break;
        case SDL_EventType::SDL_MOUSEMOTION:
            mouse_.move({ e.motion.x, e.motion.y });
            dispatcher_->trigger<j_mouse_move_event>(
                j_vec2<int32_t> { e.motion.x, e.motion.y }
            );
            break;
        }
    }
}

const j_mouse_state& j_input_system::mouse() const {
    return mouse_;
}

const j_keyboard_state& j_input_system::keyboard() const {
    return keyboard_;
}
