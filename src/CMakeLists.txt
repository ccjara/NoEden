add_executable(
    jaralyn
    gen/rng.cxx
    ../deps/easyloggingpp/easyloggingpp.cxx
    fps_provider.hxx
    
    gen/room_gen.cxx
    layer.cxx
    frame_buffer.cxx
    main.cxx
    "engine/engine.cxx"
    "engine/framework/object.cxx"
    "engine/framework/scene.cxx"
    "engine/framework/task.hxx"
    "engine/managers/env/env_manager.cxx"
    "engine/managers/env/root_config.hxx"
    "engine/managers/platform/window.cxx"
    "engine/managers/platform/platform_manager.cxx"
    "engine/managers/platform/platform_events.hxx"
    "engine/managers/resource/resource_manager.cxx"
    "engine/managers/task/task_manager.cxx"
    "systems/game_system.hxx"
    "systems/gfx/gfx_scene.cxx"
    "systems/gfx/gfx_system.cxx"
    "systems/gfx/renderer.cxx"
    "systems/gfx/text_renderer.cxx"
    "systems/input/input_system.cxx"
    "systems/input/mouse.cxx"
)

target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)