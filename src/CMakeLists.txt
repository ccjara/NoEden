add_executable(
    jaralyn
    "../deps/easyloggingpp/easyloggingpp.cxx"
    
    "components.hxx"
    "engine.cxx"
    "grid.hxx"

    "main.cxx"
    "manager_provider.cxx"
    
    "env/clock.cxx"
    "env/env_event.hxx"
    "env/env_event_dispatcher.cxx"
    "env/env_manager.cxx"
    "env/root_config.hxx"
    "env/window.cxx"

    "resource/resource_loader.hxx"
    "resource/resource_manager.cxx"

    "gfx/display.cxx"
    "gfx/fps_provider.hxx"
    "gfx/gfx_system.cxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"
    
    "input/input_system.cxx"
    "input/mouse.cxx"

    "scenes/scene.cxx"
    "scenes/world_scene.cxx"
    "system.cxx"
)
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)