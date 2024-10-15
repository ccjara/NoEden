cmake_minimum_required(VERSION 3.16)
project(NoEden)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Lua REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SPDLOG_FMT_EXTERNAL ON)
set(SPDLOG_NO_EXCEPTIONS ON)
set(EDENJSON_TESTS OFF)

add_subdirectory(test)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<$<CONFIG:Debug>:/bin>$<$<CONFIG:Release>:/rel>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<$<CONFIG:Debug>:/bin>$<$<CONFIG:Release>:/rel>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<$<CONFIG:Debug>:/bin>$<$<CONFIG:Release>:/rel>")

include(deps/cxx_flags.cmake)
include(deps/stb.cmake)
include(deps/imgui.cmake)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DNOEDEN_RELEASE)
    set(NOEDEN_RELEASE TRUE)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_subdirectory(src)

set(BUILD_SHARED_LIBS ON)

add_subdirectory(external/fmt)
add_subdirectory(external/glm)
add_subdirectory(external/spdlog)
add_subdirectory(external/utf8cpp)
add_subdirectory(external/tomlplusplus)
add_subdirectory(external/LuaBridge3)
add_subdirectory(external/edenjson)

enable_testing()
