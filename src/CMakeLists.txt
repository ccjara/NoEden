cmake_minimum_required(VERSION 3.14.1)
project(Trinias)

set (CMAKE_CXX_STANDARD 17)
set (OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED COMPONENTS SDL2::SDL2)
find_package(OpenGL REQUIRED)
include_directories(Trinias ${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(
    Trinias
    Core.cpp
    EntityAbstract.cpp
    EventHandler.cpp
    EventManager.cpp
    Game.cpp
    IdGenerator.cpp
    Layer.cpp
    MainMenu.cpp
    Renderer.cpp
    RoomGen.cpp
    StaticId.cpp
    TextRenderer.cpp
    TextTester.cpp
    Window.cpp
    main.cpp
)

target_link_libraries(Trinias SDL2::SDL2)
target_link_libraries(Trinias ${OPENGL_LIBRARIES})
