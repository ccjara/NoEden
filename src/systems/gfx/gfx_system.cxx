#include "gfx_system.hxx"

namespace detail {
    gfx_system_impl::gfx_system_impl(const window *w) {
        scene_ = std::make_unique<gfx_scene>();

        glewInit();

        window_ = w;

        renderer_->bind(window_);
    }
}
