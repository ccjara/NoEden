#ifndef JARALYN_SYSTEM_MODULE_HXX
#define JARALYN_SYSTEM_MODULE_HXX

#include "../engine/framework/game_system.hxx"

class system_module {
public:
    virtual std::unique_ptr<game_system> load(const manager_provider& managers) = 0;
    virtual ~system_module() = default;
};

#endif
