#ifndef JARALYN_GFX_MODULE_HXX
#define JARALYN_GFX_MODULE_HXX

#include "../system_module.hxx"
#include "gfx_system.hxx"

class gfx_module : public system_module {
public:
    std::unique_ptr<game_system> load(const manager_provider& managers_) override;
};

#endif 