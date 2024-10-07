set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/external)

add_library(stb_image SHARED ${CMAKE_SOURCE_DIR}/deps/stb/stb_image_implementation.cxx)
target_include_directories(stb_image PUBLIC ${INCLUDE_DIRS})

add_library(stb_perlin SHARED ${CMAKE_SOURCE_DIR}/deps/stb/stb_perlin_implementation.cxx)
target_include_directories(stb_perlin PUBLIC ${INCLUDE_DIRS})
