add_executable(
    jaralyn
    "../deps/imgui/imgui.cpp"
    "../deps/imgui/imgui_demo.cpp"
    "../deps/imgui/imgui_draw.cpp"
    "../deps/imgui/imgui_impl_sdl.cpp"
    "../deps/imgui/imgui_impl_opengl3.cpp"
    "../deps/imgui/imgui_tables.cpp"
    "../deps/imgui/imgui_widgets.cpp"

    "actor/action.hxx"
    "actor/action_queue.cxx"
    "actor/actor.cxx"
    "actor/archetype.hxx"
    "actor/player_controller.cxx"
    "actor/walk_action.cxx"

    "ai/ai_node.hxx"
    "ai/ai_walk.hxx"

    "events/event_manager.hxx"

    "game/config.hxx"
    "game/game.cxx"
    "game/game_factory.cxx"
    "game/platform_event.hxx"
    "game/window.cxx"

    "gfx/display_cell.hxx"
    "gfx/color.hxx"
    "gfx/display.cxx"
    "gfx/gfx_event.hxx"
    "gfx/renderer.cxx"
    "gfx/shader.cxx"
    "gfx/surface.cxx"
    "gfx/text_shader.cxx"
    "gfx/texture.cxx"
    "gfx/ui_renderer.cxx"

    "grid.hxx"

    "log/log.cxx"
    "log/memory_sink.cxx"

    "main.cxx"

    "math/bresenham.hxx"
    "math/rect.hxx"
    "math/vec2.hxx"

    "ui/ui.cxx"
    "ui/ui_node.cxx"
    "ui/ui_tree.cxx"
    "ui/ui_window.cxx"

    "input/input.cxx"
    "input/input_event.hxx"
    "input/input_state.cxx"
    "input/key.hxx"
    "input/keyboard_state.cxx"
    "input/mouse_button.hxx"
    "input/mouse_state.cxx"

    "scene/fov.cxx"
    "scene/scene.cxx"
    "scene/scene_events.hxx"
    "scene/tile.cxx"
    "scene/tile_builder.cxx"

    "scripts/api/api_registrar.cxx"
    "scripts/api/lua_api.hxx"
    "scripts/api/scene_api.cxx"
    "scripts/api/ui/ui_api.cxx"
    
    "scripts/script.cxx"
    "scripts/script_event.hxx"
    "scripts/script_util.hxx"
    "scripts/scripting.cxx"

    "xray/log_xray.cxx"
    "xray/scene_xray.cxx"
    "xray/script_xray.cxx"
    "xray/ui_xray.cxx"
    "xray/xray.cxx"
    "xray/xray_event.hxx"
    "xray/xray_interface.hxx"
    "xray/xray_style.hxx"
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
