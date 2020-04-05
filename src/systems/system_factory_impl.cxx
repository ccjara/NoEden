#include "system_factory_impl.hxx"

std::unique_ptr<game_system> system_factory_impl::create(system_id_t id, const manager_provider& managers) {
    switch (id) {
    case system_id::gfx:
        return create_gfx_system(managers);
    case system_id::input:
        return create_input_system(managers);
    default:
        LOG(ERROR) << "Unexpected system id " << id;
        throw;
    }
}

std::unique_ptr<game_system> system_factory_impl::create_gfx_system(const manager_provider& managers) {
    auto sys {
        std::make_unique<detail::gfx_system_impl>(
            &managers.platform->get_window()
        )
    };

    return sys;
}

std::unique_ptr<game_system> system_factory_impl::create_input_system(const manager_provider& managers) {
    return std::make_unique<detail::input_system_impl>();
}
