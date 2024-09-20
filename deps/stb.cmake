add_library(stb_image SHARED ${CMAKE_SOURCE_DIR}/deps/stb/stb_image_implementation.cxx)
target_include_directories(stb_image PUBLIC ${CMAKE_SOURCE_DIR}/external/stb)

add_library(stb_perlin SHARED ${CMAKE_SOURCE_DIR}/deps/stb/stb_perlin_implementation.cxx)
target_include_directories(stb_perlin PUBLIC ${CMAKE_SOURCE_DIR}/external/stb)
