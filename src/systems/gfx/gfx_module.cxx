#include "gfx_module.hxx"

std::unique_ptr<game_system> gfx_module::load(const manager_provider& managers_)
{
    const window* w { &managers_.platform->get_window() };

    auto gfx { std::make_unique<detail::gfx_system_impl>(w) };

    return gfx;
}
