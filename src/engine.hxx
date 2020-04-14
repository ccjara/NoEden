#ifndef JARALYN_ENGINE_HXX
#define JARALYN_ENGINE_HXX

#include "manager_provider.hxx"
#include "gfx/gfx_system.hxx"

class j_engine {
private:
    std::unique_ptr<j_gfx_system> gfx_system_ { nullptr };

    j_manager_provider managers_;
public:
    /**
     * @brief Runs the engine together will all game systems.
     */
    void run();
};

#endif 