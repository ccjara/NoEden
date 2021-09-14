#include "input.hxx"

Input::Input(entt::dispatcher& dispatcher) : dispatcher_ { dispatcher } {
    dispatcher_.sink<XrayFocusEvent>().connect<&Input::on_xray_focus>(this);
}

void Input::poll_platform() {
    SDL_Event e;

    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_KEYDOWN, SDL_EventType::SDL_MOUSEWHEEL)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
        if (input_blocked_) {
            continue;
        }
        switch (e.type) {
            case SDL_EventType::SDL_KEYDOWN: {
                const auto key { static_cast<Key>(e.key.keysym.sym) };
                state_.keyboard_.key_down(key);
                dispatcher_.trigger<KeyDownEvent>(key, &state_);
                break;
            }
            case SDL_EventType::SDL_KEYUP: {
                const auto key { static_cast<Key>(e.key.keysym.sym) };
                state_.keyboard_.key_up(key);
                dispatcher_.trigger<KeyUpEvent>(key, &state_);
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
                const auto button { static_cast<MouseButton>(e.button.button) };
                state_.mouse_.mouse_down(button);
                dispatcher_.trigger<MouseDownEvent>(button, &state_);
                break;
            }
            case SDL_EventType::SDL_MOUSEBUTTONUP: {
                const auto button { static_cast<MouseButton>(e.button.button) };
                state_.mouse_.mouse_up(button);
                dispatcher_.trigger<MouseUpEvent>(button, &state_);
                break;
            }
            case SDL_EventType::SDL_MOUSEMOTION: {
                const Vec2<i32> pos { e.motion.x, e.motion.y };
                state_.mouse_.move(pos);
                dispatcher_.trigger<MouseMoveEvent>(pos, &state_);
                break;
            }
        }
    }
}

void Input::on_xray_focus(const XrayFocusEvent& e) {
    input_blocked_ = e.focused;
}

const InputState& Input::state() const {
    return state_;
}
