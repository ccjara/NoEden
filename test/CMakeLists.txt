find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

enable_testing()

add_executable(jaratest
    "input/input.cxx"
    "lang/translator.cxx"
    "lang/translation_loader.cxx"
    "math/vec2.cxx"
)

target_include_directories(jaratest PRIVATE ${catch2_SOURCE_DIR}/include)
target_include_directories(jaratest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(jaratest PRIVATE jarastatic Catch2::Catch2WithMain)
target_precompile_headers(jaratest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hxx)

catch_discover_tests(jaratest)


