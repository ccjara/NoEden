#include "main_menu/main_menu_controller.hxx"
#include "main_menu/main_menu.hxx"

MainMenuController::MainMenuController(MainMenu* menu, EventManager* events) : menu_(menu), events_(events) {
    assert(events_);
    assert(menu_);

    key_down_sub_ = events_->on<KeyDownEvent>(this, &MainMenuController::on_key_down);
}

EventResult MainMenuController::on_key_down(const KeyDownEvent& e) {
    if (e.key == Key::Up) {
        menu_->activate_previous_item();
        return EventResult::Halt;
    }
    if (e.key == Key::Down) {
        menu_->activate_next_item();
        return EventResult::Halt;
    }

    return EventResult::Continue;
}
