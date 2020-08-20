#include "base_scene.hxx"

void j_base_scene::attach(
    j_scene_writer* const writer,
    entt::dispatcher* const game_events
) {
    assert(writer);
    assert(game_events);
    scene_writer_ = writer;
    game_events_ = game_events;
}

void j_base_scene::on_create() {
}

j_scene_type j_base_scene::type() const noexcept {
    return type_;
}

bool j_base_scene::opaque() const noexcept {
    return is_opaque_;
}

bool j_base_scene::blocking() const noexcept {
    return is_blocking_;
}
