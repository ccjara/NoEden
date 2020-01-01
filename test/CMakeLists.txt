add_executable(
    jaralyn_test
    jaralyn.test.cxx
    main.cxx
)

find_package(Catch2 REQUIRED)

target_link_libraries(jaralyn_test Catch2::Catch2 jaralyn_static)
target_include_directories(jaralyn_test PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_precompile_headers(jaralyn_bin REUSE_FROM jaralyn_static)

include(Catch)

add_test(NAME jaralyn_test COMMAND jaralyn_test -o report.xml)
