#include "input_scene.hxx"

input_scene::input_scene() {
    primary_task_ = std::make_unique<input_task>();
}
