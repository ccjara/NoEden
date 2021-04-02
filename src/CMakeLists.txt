add_executable(
    jaralyn
    "../deps/easyloggingpp/easyloggingpp.cxx"

    "../deps/imgui/imgui.cpp"
    "../deps/imgui/imgui_demo.cpp"
    "../deps/imgui/imgui_draw.cpp"
    "../deps/imgui/imgui_impl_sdl.cpp"
    "../deps/imgui/imgui_impl_opengl3.cpp"
    "../deps/imgui/imgui_tables.cpp"
    "../deps/imgui/imgui_widgets.cpp"

    "components/attribute.hxx"
    "components/attribute_bearing.cxx"
    "components/components.hxx"

    "env/clock.hxx"
    "env/env_manager.cxx"
    "env/root_config.hxx"
    "env/window.cxx"

    "event/gfx_event.hxx"
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

    "gfx/color.hxx"
    "gfx/display.cxx"
    "gfx/fps_provider.hxx"
    "gfx/gfx_system.cxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"
    "gfx/ui_renderer.cxx"
    "gfx/world_renderer.cxx"

    "hud/journal.cxx"
    "hud/hud_system.cxx"
    "hud/ui_node.cxx"
    "hud/ui_node_proxy.hxx"
    "hud/ui_node_proxy_interface.hxx"
    "hud/ui_proxy.cxx"
    "hud/ui_lua_dependency_store.hxx"
    "hud/ui_tree.hxx"
    "hud/ui_window.cxx"
    "hud/ui_window_proxy.hxx"

    "identity.hxx"

    "input/input_system.cxx"
    "input/keyboard.cxx"
    "input/keyboard_state.hxx"
    "input/mouse.cxx"
    "input/mouse_state.hxx"

    "player/player_system.cxx"
    "pagination.hxx"

    "scripts/script.cxx"
    "scripts/script_system.cxx"

    "system.hxx"
    "system_interface.cxx"
    "systems.cxx"

    "unit/unit_system.cxx"

    "xray/script_xray.cxx"
    "xray/ui_xray.cxx"
    "xray/xray_interface.hxx"
    "xray/xray_style.hxx"
    "xray/xray_system.cxx"
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
