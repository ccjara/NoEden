add_executable(
    jaralyn
    "../deps/easyloggingpp/easyloggingpp.cxx"

    "components/attribute.hxx"
    "components/attribute_bearing.cxx"
    "components/components.hxx"
    "components/inventory.cxx"

    "engine.cxx"
    "grid.hxx"
    "identity.hxx"
    "main.cxx"

    "math/bresenham.hxx"
    "math/rect.hxx"
    "math/vec2.hxx"

    "env/env_event_system.cxx"
    "env/env_interface.hxx"
    "env/env_manager.cxx"
    "env/root_config.cxx"
    "env/window.cxx"

    "game_event.hxx"

    "gfx/display.cxx"
    "gfx/fps_provider.hxx"
    "gfx/gfx_system.cxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"

    "input/input_interface.hxx"
    "input/input_system.cxx"
    "input/keyboard.cxx"
    "input/mouse.cxx"

    "scenes/base_scene.cxx"
    "scenes/inventory_scene.cxx"
    "scenes/null_scene.hxx"
    "scenes/scene_composer.cxx"
    "scenes/scene_interface.hxx"
    "scenes/status_scene.cxx"
    "scenes/world_scene.cxx"

    "scripts/script.cxx"
    "scripts/script_system.cxx"
)

target_link_libraries(jaralyn ${LUA_LIBRARIES})
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)
