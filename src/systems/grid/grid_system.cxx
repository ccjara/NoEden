#include "grid_system.hxx"

namespace detail {
    grid_system_impl::grid_system_impl() {
        scene_ = std::make_unique<grid_scene>();
    }

    void grid_system_impl::attach(change_controller& controller) {
        LOG(INFO) << "Unit system attaching to change controller";
    }

    void grid_system_impl::on_systems_loaded() {
        current_.resize({ 5, 5 });

        for (auto y { 0U }; y < 5; y++) {
            for (auto x { 0U }; x < 5; x++) {
                if (x == 0 || y == 0 || y == 4 || x == 4) {
                    current_.set({ x, y }, 1);
                }
            }
        }

        current_.prnt();
    }
}

