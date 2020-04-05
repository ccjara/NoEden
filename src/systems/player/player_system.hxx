#ifndef JARALYN_PLAYER_SYSTEM_HXX
#define JARALYN_PLAYER_SYSTEM_HXX

#include "../../engine/framework/game_system.hxx"
#include "../system_definition.hxx"

class player_system : public game_system {
protected:
public:
    system_id_t id() const noexcept override {
        return static_cast<system_id_t> (system_id::player);
    };
};


namespace detail {
    class player_system_impl : public player_system {
    private:
        position<uint32_t> position_;
    public:
        void attach(change_controller& controller) override;
    };
}

#endif