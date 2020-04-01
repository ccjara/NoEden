#include "mouse.hxx"

position<int32_t> mouse_t::pos() const noexcept {
    return position_;
}

void mouse_t::move(position<int32_t> pos) noexcept {
    position_ = pos;
}

int32_t mouse_t::x() const noexcept {
    return position_.x;
}

int32_t mouse_t::y() const noexcept {
    return position_.y;
}
