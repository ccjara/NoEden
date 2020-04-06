#include "unit_scene.hxx"

unit_scene::unit_scene() {
    primary_task_ = std::make_unique<unit_task>();
}
