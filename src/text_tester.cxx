#include "text_tester.hxx"

text_tester::text_tester() {
    SDL_StartTextInput();
}

text_tester::~text_tester() noexcept {
    SDL_StopTextInput();
}

void text_tester::on_sdl_event(const SDL_Event& e) {
    if (e.type == SDL_TEXTINPUT) {
        input += e.text.text;
    } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (input.size() > 0) {
                    input.pop_back();
                }
                return;
            case SDLK_RETURN:
                input += "|n";
        }
    }
}

const char *text_tester::get_text() const {
    return input.c_str();
}