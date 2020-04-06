#include "gfx_system.hxx"

namespace detail {
    gfx_system_impl::gfx_system_impl(const window *w) {
        scene_ = std::make_unique<gfx_scene>(renderer_.get());

        window_ = w;

        renderer_->bind(window_);
    }
}
