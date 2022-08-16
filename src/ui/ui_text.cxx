#include "ui_text.hxx"

UiText::UiText() {
    type_ = UiNodeType::text;
}

std::string_view UiText::text() const {
    return text_;
}

void UiText::set_text(std::string_view text) {
    text_ = text;
}
