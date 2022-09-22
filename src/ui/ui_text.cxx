#include "ui_text.hxx"

UiText::UiText() : text_(std::make_unique<Text>()) {
    type_ = UiNodeType::text;
}

Text& UiText::text() const {
    return *text_.get();
}

void UiText::resize(Vec2<u32> size) {
    size_ = size;
    text_->set_clamp(size);
    text_->update();
}

void UiText::resize(u32 width, u32 height) {
    resize(Vec2<u32>(width, height));
}

void UiText::set_width(u32 width) {
    resize(Vec2<u32>(width, size_.y));
}

void UiText::set_height(u32 height) {
    resize(Vec2<u32>(size_.x, height));
}