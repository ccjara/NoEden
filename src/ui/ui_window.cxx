#include "ui_window.hxx"

UiWindow::UiWindow() {
    type_ = UiNodeType::window;
}

std::string_view UiWindow::title() const {
    return title_;
}

void UiWindow::set_title(std::string_view title) {
    title_ = title;
}
