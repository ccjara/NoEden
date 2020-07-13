#ifndef JARALYN_ENGINE_HXX
#define JARALYN_ENGINE_HXX

#include "env/env_manager.hxx"
#include "gfx/gfx_system.hxx"
#include "input/input_system.hxx"
#include "scripts/script_system.hxx"
#include "scenes/scene_composer.hxx"
#include "scenes/world_scene.hxx"

class j_engine {
private:
    std::unique_ptr<j_env_manager> env_ { nullptr };
    std::unique_ptr<j_gfx_system> gfx_system_ { nullptr };
    std::unique_ptr<j_input_system> input_system_ { nullptr };
    std::unique_ptr<j_script_system> script_system_ { nullptr };

    j_scene_composer composer_;
public:
    /**
     * @brief Runs the engine together will all game systems.
     */
    void run();
};

#endif 