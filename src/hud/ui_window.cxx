#include "ui_window.hxx"

j_ui_window::j_ui_window() {
    type_ = j_ui_node_type::window;
}

std::string_view j_ui_window::title() const {
    return title_;
}

void j_ui_window::set_title(std::string_view title) {
    title_ = title;
}
