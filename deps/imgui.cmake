if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_library(imgui SHARED
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_sdl2.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
    )

    target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/external/imgui)
    target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})

    target_link_libraries(imgui PUBLIC OpenGL::GL)
endif()
