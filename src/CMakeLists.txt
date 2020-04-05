add_executable(
    jaralyn
    ../deps/easyloggingpp/easyloggingpp.cxx
    fps_provider.hxx
    layer.cxx
    frame_buffer.cxx
    main.cxx
    "engine/engine.cxx"
    "engine/framework/game_system.cxx"
    "engine/framework/object.cxx"
    "engine/framework/scene.cxx"
    "engine/framework/scene_loader.cxx"
    "engine/framework/scene_factory.hxx"
    "engine/framework/system_scene.cxx"
    "engine/framework/system_factory.hxx"
    "engine/framework/task.hxx"
    "engine/framework/universal_object.cxx"
    "engine/framework/universal_scene.cxx"
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
    "systems/gfx/gfx_system.cxx"
    "systems/gfx/renderer.cxx"
    "systems/gfx/text_renderer.cxx"
    "systems/input/input_system.cxx"
    "systems/input/mouse.cxx"
    "systems/player/player_system.cxx"
    "systems/system_factory_impl.cxx"
)
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)