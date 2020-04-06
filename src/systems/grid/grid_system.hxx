#ifndef JARALYN_GRID_SYSTEM_HXX
#define JARALYN_GRID_SYSTEM_HXX

#include "../../engine/framework/game_system.hxx"
#include "../system_definition.hxx"
#include "grid.hxx"
#include "grid_scene.hxx"

class grid_system : public game_system {
protected:
    grid current_;
public:
    system_id_t id() const noexcept override {
        return static_cast<system_id_t> (system_id::grid);
    };
};


namespace detail {
    class grid_system_impl : public grid_system {
    public:
        grid_system_impl();

        void attach(change_controller& controller) override;

        void on_systems_loaded() override;
    };
}

#endif