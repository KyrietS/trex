cmake_minimum_required(VERSION 3.10)
project(trex_tests)

include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
    TestAtlas.cpp
    TestBitmapHelpers.cpp
    TestFont.cpp
    TestTextShaper.cpp
    TestCharset.cpp
)

# trex
target_link_libraries(${PROJECT_NAME} trex)

# Google Test
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock)
gtest_discover_tests(${PROJECT_NAME})

# Copy fonts from examples
add_custom_target(copy_fonts
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../examples/fonts ${CMAKE_CURRENT_BINARY_DIR}/fonts
)
add_dependencies(${PROJECT_NAME} copy_fonts)
