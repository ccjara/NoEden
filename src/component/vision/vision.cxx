#include "component/vision/vision.hxx"

void Vision::set_vision_radius(i32 radius) {
    fov_.set_vision_radius(std::max(radius, 1));
}

i32 Vision::vision_radius() const {
    return fov_.vision_radius();
}

const Fov& Vision::fov() const {
    return fov_;
}

Fov& Vision::fov() {
    return fov_;
}

bool Vision::applied() const {
    return apply_;
}

void Vision::set_apply(bool apply_vision) {
    apply_ = apply_vision;
}

void Vision::on_player_attached() {
    apply_ = true;
}

void Vision::on_player_detached() {
    apply_ = false;
}
