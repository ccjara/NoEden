#include "ui_window.hxx"

std::string_view j_ui_window::title() const {
    return title_;
}

void j_ui_window::set_title(std::string_view title) {
    title_ = title;
}
