add_executable(
    jaralyn
    "../deps/easyloggingpp/easyloggingpp.cxx"
    
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

    "gfx/display.hxx"
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
    "system.cxx"
  "gfx/display.cxx")
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)