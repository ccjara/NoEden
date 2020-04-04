#ifndef JARALYN_SYSTEM_FACTORY_IMPL_HXX
#define JARALYN_SYSTEM_FACTORY_IMPL_HXX

#include "../engine/framework/game_system.hxx"
#include "../engine/framework/system_factory.hxx"
#include "../engine/managers/manager_provider.hxx"
#include "system_definition.hxx"
#include "gfx/gfx_system.hxx"
#include "input/input_system.hxx"

class system_factory_impl : public system_factory {
private:
    std::unique_ptr<game_system> create_gfx_system(const manager_provider& managers);
    std::unique_ptr<game_system> create_input_system(const manager_provider& managers);
public:
    [[nodiscard]] virtual std::unique_ptr<game_system> create(system_id_t id, const manager_provider& managers) override;
};

#endif
