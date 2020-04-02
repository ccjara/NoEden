#include "gfx_system.hxx"

namespace detail {
    gfx_system_impl::gfx_system_impl(const window *w) {
        glewInit();

        window_ = w;

        renderer_->bind(window_);
    }

    void gfx_system_impl::public_poc_method() {
        LOG(DEBUG) << __FUNCTION__ << ": Hellow";
    }
}
