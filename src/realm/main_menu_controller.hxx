#ifndef NOEDEN_MAIN_MENU_CONTROLLER_HXX
#define NOEDEN_MAIN_MENU_CONTROLLER_HXX

class MainMenu;

class MainMenuController {
public:
    MainMenuController(MainMenu* menu, EventManager* events);
private:
    MainMenu* menu_ = nullptr;
    EventManager* events_ = nullptr;

    EventResult on_key_down(const KeyDownEvent& e);
    Subscription<KeyDownEvent> key_down_sub_;
};

#endif
