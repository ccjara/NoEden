find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

enable_testing()

add_executable(NoEdenTest
    "input/input.cxx"
    "lang/translator.cxx"
    "lang/translation_loader.cxx"
    "math/vec2.cxx"
)

target_include_directories(NoEdenTest PRIVATE ${catch2_SOURCE_DIR}/include)
target_include_directories(NoEdenTest PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(NoEdenTest PRIVATE NoEdenStatic Catch2::Catch2WithMain)
target_precompile_headers(NoEdenTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hxx)

catch_discover_tests(NoEdenTest)


