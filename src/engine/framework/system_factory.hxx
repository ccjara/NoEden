#ifndef JARALYN_SYSTEM_FACTORY_HXX
#define JARALYN_SYSTEM_FACTORY_HXX

#include "game_system.hxx"
#include "../managers/manager_provider.hxx"

class system_factory {
public:
    [[nodiscard]] virtual std::unique_ptr<game_system> create(system_id_t id, const manager_provider& managers) = 0;
};

#endif