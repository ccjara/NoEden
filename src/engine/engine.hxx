#ifndef JARALYN_ENGINE_HXX
#define JARALYN_ENGINE_HXX

#include "managers/manager_provider.hxx"

#include "framework/system_factory.hxx"
#include "framework/scene_loader.hxx"
#include "framework/scene_factory.hxx"

class engine {
private:
    manager_provider managers_;
    scene_loader scene_loader_;

    system_factory* const sys_factory_;
public:
    /**
     * @brief Constructs the engine with game specific parameters
     */
    engine(system_factory* const sys_factory, scene_factory* const sc_factory);

    /**
     * @brief Runs the engine together will all game systems.
     */
    void run();
};

#endif 