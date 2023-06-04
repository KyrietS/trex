cmake_minimum_required(VERSION 3.5)

project(trex)

set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT trex)

file(GLOB SOURCES "src/*.cpp" "src/*.hpp")
add_library(trex STATIC ${SOURCES})

target_include_directories(trex PUBLIC src/)
target_include_directories(trex PRIVATE lib/stb)

add_subdirectory(lib/freetype) # Freetype
set(HB_BUILD_SUBSET OFF)
add_subdirectory(lib/harfbuzz) # Harfbuzz

target_link_libraries(trex freetype harfbuzz)
