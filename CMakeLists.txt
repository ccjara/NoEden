cmake_minimum_required(VERSION 3.16)
project(jaralyn)

set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED COMPONENTS SDL2::SDL2)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(jaralyn deps)
include_directories(jaralyn ${SDL2_INCLUDE_DIRS})
include_directories(jaralyn ${OPENGL_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(bin)
add_subdirectory(test)

