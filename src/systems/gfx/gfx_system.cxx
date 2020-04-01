#include "gfx_system.hxx"

void gfx_system::on_load(const manager_provider& managers_) {
    glewInit();

    window_ = &managers_.platform->get_window();

    renderer_->bind(window_);
}
