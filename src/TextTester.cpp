#include "stdafx.h"

TextTester::TextTester(EventManager &events) :
    onTextInputHandler(
        std::make_shared<EventHandler>(
            SDL_TEXTINPUT,
            std::bind(&TextTester::onTextInput, this, std::placeholders::_1)
        )
    ),
    onKeyPressHandler(
        std::make_shared<EventHandler>(
            SDL_KEYDOWN,
            std::bind(&TextTester::onKeyPress, this, std::placeholders::_1)
        )
    )
{
    events.attach(onTextInputHandler);
    events.attach(onKeyPressHandler);
    SDL_StartTextInput();
}

void TextTester::onTextInput(const SDL_Event& e) {
    input += e.text.text;
}

void TextTester::onKeyPress(const SDL_Event& e) {
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

const char *TextTester::getText() const {
    return input.c_str();
}