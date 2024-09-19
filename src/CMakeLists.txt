include_directories(${PROJECT_SOURCE_DIR}/src)

set(DEPS
    "../deps/stb/stb_image_implementation.cxx"
    "../deps/stb/stb_perlin_implementation.cxx"
)

if (NOT NOEDEN_RELEASE)
    set(DEPS ${DEPS}
        "../deps/imgui/imgui.cpp"
        "../deps/imgui/imgui_demo.cpp"
        "../deps/imgui/imgui_draw.cpp"
        "../deps/imgui/imgui_impl_sdl.cpp"
        "../deps/imgui/imgui_impl_opengl3.cpp"
        "../deps/imgui/imgui_tables.cpp"
        "../deps/imgui/imgui_widgets.cpp"
    )
endif()

set(SOURCES
    action/action.cxx
    action/action_creator.hxx
    action/action_event.hxx
    action/action_processor.hxx
    action/action_queue.cxx
    action/action_result.hxx
    action/action_type.hxx
    action/move_action.cxx
    action/null_action.hxx

    ai/ai_context.cxx
    ai/ai_condition.cxx
    ai/ai_closest_entity.cxx
    ai/ai_node.cxx
    ai/ai_selector.cxx
    ai/ai_sequence.cxx
    ai/ai_script.cxx
    ai/ai_walk.cxx
    ai/blackboard.cxx
    ai/condition_resolver.cxx
    ai/generic_ai_node.hxx

    catalog/catalog.cxx

    component/behavior.cxx
    component/component.cxx
    component/generic_component.cxx
    component/life.cxx
    component/render.cxx
    component/skills.cxx
    component/vision/vision.cxx
    component/vision/fov.cxx

    config/config.hxx
    config/config_event.hxx
    config/config_manager.cxx

    entity/archetype.hxx
    entity/entity.cxx
    entity/entity_event.hxx
    entity/entity_manager.cxx

    framework/concepts.hxx
    framework/event_type.hxx
    framework/event.hxx
    framework/event_manager.hxx
    framework/grid.hxx
    framework/noise_generator.cxx
    framework/service_locator.hxx
    framework/usings.hxx

    game/exit_manager.cxx

    gfx/color.hxx
    gfx/display_cell.hxx
    gfx/display_info.hxx
    gfx/display.cxx
    gfx/gfx_event.hxx
    gfx/renderer.cxx
    gfx/shader.cxx
    gfx/image.cxx
    gfx/text.cxx
    gfx/text_shader.cxx
    gfx/texture.cxx

    input/input.cxx
    input/input_event.hxx
    input/input_reader.hxx
    input/key.hxx
    input/keyboard_state.cxx
    input/mouse_button.hxx
    input/mouse_state.cxx

    instrumentation/perf_timer.cxx
    instrumentation/profiler.cxx

    log/log.cxx

    math/bresenham.hxx
    math/rect.hxx
    math/vec2.hxx

    lang/dictionary.cxx
    lang/translator.cxx
    lang/translation_loader.cxx

    platform/platform_event.hxx
    platform/platform.cxx

    main_menu/main_menu.cxx
    main_menu/main_menu_controller.cxx
    main_menu/main_menu_realm.cxx
    realm/realm.cxx
    realm/realm_manager.cxx
    realm/realm_type.cxx

    scripts/api/catalog_api.cxx
    scripts/api/config_api.cxx
    scripts/api/game_api.cxx
    scripts/api/log_api.cxx
    scripts/api/lua_api.hxx
    scripts/api/realm_api.cxx
    scripts/api/scene_api.cxx
    scripts/api/script_api.cxx
    scripts/api/ui/ui_api.cxx

    scripts/script.cxx
    scripts/script_event.hxx
    scripts/script_loader.cxx
    scripts/script_registry.cxx
    scripts/scripting.cxx

    ui/ui.cxx
    ui/ui_node.cxx
    ui/ui_text.cxx
    ui/ui_tree.cxx
    ui/ui_window.cxx

    world/camera.cxx
    world/camera_controller.cxx
    world/chunk_generator.cxx
    world/chunk_manager.cxx
    world/chunk_pos.cxx
    world/player_controller.cxx
    world/tile.hxx
    world/tile_accessor.cxx
    world/world_context.hxx
    world/world_realm.cxx
    world/world_event.hxx
    world/world_spec_creator.cxx
    world/world_pos.cxx
    world/world_spec.cxx
    world/vision_manager.cxx
)

if (NOT NOEDEN_RELEASE)
    set(SOURCES ${SOURCES}
        xray/log_xray.cxx
        xray/noise_texture.cxx
        xray/perf_xray.cxx
        xray/scene_xray.cxx
        xray/script_xray.cxx
        xray/ui_xray.cxx
        xray/xray.hxx
        xray/xray_manager.cxx
        xray/xray_style.hxx
    )
endif ()


set(SOURCES ${DEPS} ${SOURCES})

set(LIBRARIES
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${LUA_LIBRARIES}
    ${GLEW_LIBRARIES}
)

add_executable(NoEden ${SOURCES} "game/game.cxx" "main.cxx")
target_compile_options(NoEden PRIVATE ${CMAKE_CXX_FLAGS})
target_link_libraries(NoEden ${LIBRARIES})

target_precompile_headers(NoEden PRIVATE pch.hxx)

# copy script sources to runtime dir
add_custom_command(
    TARGET NoEden POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/scripts/lua
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/scripts
)

# also assets
add_custom_command(
    TARGET NoEden POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
)

# language dictionaries
add_custom_command(
    TARGET NoEden POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/lang/dictionaries
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dictionaries
)
