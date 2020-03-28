add_executable(
    jaralyn
    gen/rng.cxx
    ../deps/easyloggingpp/easyloggingpp.cxx
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
    main.cxx
)

target_link_libraries(jaralyn SDL2::SDL2)
target_link_libraries(jaralyn ${OPENGL_LIBRARIES})
target_link_libraries(jaralyn GLEW::GLEW)

target_precompile_headers(jaralyn PRIVATE stdafx.hxx)