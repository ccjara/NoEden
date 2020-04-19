#include "base_scene.hxx"

void j_base_scene::attach(j_scene_writer* const writer) {
    assert(writer);
    scene_writer_ = writer;
}

bool j_base_scene::opaque() const noexcept {
    return is_opaque_;
}

bool j_base_scene::blocking() const noexcept {
    return is_blocking_;
}
