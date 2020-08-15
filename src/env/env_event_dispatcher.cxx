#include "env_event_dispatcher.hxx"

j_env_event_dispatcher::j_env_event_dispatcher(entt::dispatcher* const dispatcher) :
    dispatcher_(dispatcher) {
    assert(dispatcher_);
}

void j_env_event_dispatcher::listen() const noexcept {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
        case SDL_EventType::SDL_QUIT:
            dispatcher_->trigger<j_quit_event>(j_quit_event());
            return;
        case SDL_EventType::SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                dispatcher_->trigger(j_resize_event({
                    static_cast<uint32_t> (e.window.data1),
                    static_cast<uint32_t> (e.window.data2)
                }));
            }
            break;
        case SDL_EventType::SDL_KEYDOWN:
            dispatcher_->trigger(j_key_down_event(e.key.keysym.sym));
            break;
        case SDL_EventType::SDL_KEYUP:
            dispatcher_->trigger(j_key_up_event(e.key.keysym.sym));
            break;
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            dispatcher_->trigger(
                j_mouse_down_event(static_cast<j_mouse_button>(e.button.button))
            );
            break;
        case SDL_EventType::SDL_MOUSEBUTTONUP:
            dispatcher_->trigger(
                j_mouse_up_event(static_cast<j_mouse_button>(e.button.button))
            );
            break;
        case SDL_EventType::SDL_MOUSEMOTION:
            dispatcher_->trigger(j_mouse_move_event({ e.motion.x, e.motion.y }));
            break;
        }
    }
}
