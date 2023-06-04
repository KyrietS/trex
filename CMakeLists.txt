cmake_minimum_required(VERSION 3.5)

project(Trex)

set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT trex)

file(GLOB SOURCES "src/*.cpp" "src/*.hpp")
add_library(trex STATIC ${SOURCES})

target_include_directories(trex PUBLIC src/)
target_include_directories(trex PRIVATE lib/stb)

add_subdirectory(lib/freetype) # FreeType
set(HB_BUILD_SUBSET OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/harfbuzz) # HarfBuzz

target_link_libraries(trex freetype harfbuzz)
