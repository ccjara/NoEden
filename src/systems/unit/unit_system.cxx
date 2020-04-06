#include "unit_system.hxx"

namespace detail {
    unit_system_impl::unit_system_impl() {
        scene_ = std::make_unique<unit_scene>();
    }

    void unit_system_impl::attach(change_controller& controller) {
        LOG(INFO) << "Unit system attaching to change controller";
    }

    void unit_system_impl::on_load() {
    }
}

