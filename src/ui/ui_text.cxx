#include "ui_text.hxx"

UiText::UiText() : text_(std::make_unique<Text>()) {
    type_ = UiNodeType::text;
}

Text& UiText::text() const {
    return *text_;
}

void UiText::resize(glm::ivec2 size) {
    size_ = size;
    text_->set_region(size);
    text_->update();
}

void UiText::resize(i32 width, i32 height) {
    resize(glm::ivec2(width, height));
}

void UiText::set_width(i32 width) {
    resize(glm::ivec2(width, size_.y));
}

void UiText::set_height(i32 height) {
    resize(glm::ivec2(size_.x, height));
}

void UiText::update() {
    text_->update();
}
