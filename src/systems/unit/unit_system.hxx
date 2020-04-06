#ifndef JARALYN_UNIT_SYSTEM_HXX
#define JARALYN_UNIT_SYSTEM_HXX

#include "../../engine/framework/game_system.hxx"
#include "../system_definition.hxx"
#include "unit_object.hxx"
#include "unit_scene.hxx"

class unit_system : public game_system {
protected:
public:
    system_id_t id() const noexcept override {
        return static_cast<system_id_t> (system_id::unit);
    };
};


namespace detail {
    class unit_system_impl : public unit_system {
    private:
        position<uint32_t> position_;
    public:
        unit_system_impl();

        void attach(change_controller& controller) override;

        void on_load() override;
    };
}

#endif