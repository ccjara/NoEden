#include "grid_scene.hxx"

grid_scene::grid_scene() {
    primary_task_ = std::make_unique<grid_task>();
}
