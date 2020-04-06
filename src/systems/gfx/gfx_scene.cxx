#include "gfx_scene.hxx"

gfx_scene::gfx_scene() {
    primary_task_ = std::make_unique<gfx_task>();
}
