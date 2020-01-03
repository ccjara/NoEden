add_library(
    jaralyn_static
    STATIC
    gen/rng.cxx
    fps_provider.hxx
    core.cxx
    game.cxx
    layer.cxx
    renderer.cxx
    gen/room_gen.cxx
    text_renderer.cxx
    text_tester.cxx
    window.cxx
    frame_buffer.cxx
)

target_precompile_headers(jaralyn_static PRIVATE stdafx.hxx)

target_link_libraries(jaralyn_static SDL2::SDL2)
target_link_libraries(jaralyn_static ${OPENGL_LIBRARIES})
