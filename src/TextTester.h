#ifndef __TEXT_TESTER__
#define __TEXT_TESTER__

class TextTester {
private:
    std::string input;

    std::shared_ptr<EventHandler> onTextInputHandler;
    std::shared_ptr<EventHandler> onKeyPressHandler;
    void onTextInput(const SDL_Event& e);
    void onKeyPress(const SDL_Event& e);
public:
    TextTester(EventManager& events);
    
    const char *getText() const;
};

#endif
