add_executable(
    jaralyn_bin
    main.cxx
)

target_link_libraries(
    jaralyn_bin
    jaralyn_static
    SDL2::SDL2
    ${OPENGL_LIBRARIES}
)

target_precompile_headers(jaralyn_bin REUSE_FROM jaralyn_static)

target_include_directories(jaralyn_bin PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_custom_command(
    TARGET jaralyn_bin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/assets/*
    ${CMAKE_CURRENT_BINARY_DIR}
)
