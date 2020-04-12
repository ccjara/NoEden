add_executable(
    jaralyn
    ../deps/easyloggingpp/easyloggingpp.cxx
    layer.cxx
    frame_buffer.cxx
    main.cxx
    "engine/engine.cxx"
    "engine/framework/game_system.cxx"
    "engine/framework/object.cxx"
    "engine/framework/scene.cxx"
    "engine/framework/scene_loader.cxx"
    "engine/framework/scene_factory.hxx"
    
    "engine/framework/system_factory.hxx"
    "engine/framework/task.hxx"
    "engine/framework/universal_object.cxx"
    "engine/framework/universal_scene.cxx"
    "engine/managers/env/clock.cxx"
    "engine/managers/env/env_manager.cxx"
    "engine/managers/env/root_config.hxx"
    "engine/managers/manager_provider.cxx"
    "engine/managers/platform/window.cxx"
    "engine/managers/platform/platform_manager.cxx"
    "engine/managers/platform/platform_events.hxx"
    "engine/managers/resource/resource_manager.cxx"
    "engine/managers/state/change_controller.cxx"
    "engine/managers/state/state_manager.cxx"
    "engine/managers/task/task_manager.cxx"
    "scenes/scene_constants.hxx"
    "scenes/scene_factory_impl.cxx"
    "systems/gen/room_gen.cxx"
    "systems/gen/rng.cxx"
    "systems/gfx/fps_provider.hxx"
    "systems/gfx/gfx_scene.cxx"
    "systems/gfx/gfx_system.cxx"
    "systems/gfx/renderer.cxx"
    "systems/gfx/shader.cxx"
    "systems/gfx/surface.cxx"
    "systems/gfx/text_shader.cxx"
    "systems/gfx/texture.cxx"
    "systems/grid/cell.cxx"
    "systems/grid/grid.cxx"
    "systems/grid/grid_scene.cxx"
    "systems/grid/grid_system.cxx"
    "systems/grid/grid_task.hxx"
    "systems/input/input_scene.cxx"
    "systems/input/input_system.cxx"
    "systems/input/mouse.cxx"
    "systems/system_factory_impl.cxx"
    "systems/unit/unit_scene.cxx"
    "systems/unit/unit_system.cxx"
)
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)