add_executable(
    jaralyn
    "../deps/easyloggingpp/easyloggingpp.cxx"

    "components/attribute.hxx"
    "components/attribute_bearing.cxx"
    "components/components.hxx"
    "components/item.cxx"

    "controller/controller.cxx"
    "controller/controller_system.cxx"
    "controller/unit_controller.cxx"

    "color.hxx"

    "env/clock.hxx"
    "env/env_manager.cxx"
    "env/root_config.hxx"
    "env/window.cxx"

    "event/inventory_event.hxx"
    "event/platform_event.hxx"
    "event/player_event.hxx"
    "event/script_event.hxx"

    "game.cxx"
    "game_factory.cxx"

    "grid.hxx"

    "main.cxx"

    "math/bresenham.hxx"
    "math/rect.hxx"
    "math/vec2.hxx"

    "gfx/display.cxx"
    "gfx/fps_provider.hxx"
    "gfx/gfx_system.cxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"

    "hud/hud_system.cxx"

    "identity.hxx"

    "input/input_system.cxx"
    "input/keyboard.cxx"
    "input/keyboard_state.hxx"
    "input/mouse.cxx"
    "input/mouse_state.hxx"

    "player/player_system.cxx"
    "pagination.hxx"

    "scripts/display_proxy.hxx"
    "scripts/script.cxx"
    "scripts/script_system.cxx"

    "states/state.hxx"
    "states/state_stack.cxx"
    "states/state_writer.hxx"
    "states/world_state.cxx"

    "system.hxx"
    "system_interface.cxx"
    "systems.cxx"
)

target_link_libraries(jaralyn ${LUA_LIBRARIES})
target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)

# copy script sources to runtime dir
add_custom_command(
    TARGET jaralyn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/scripts/lua
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
)

# also assets
add_custom_command(
    TARGET jaralyn POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)
