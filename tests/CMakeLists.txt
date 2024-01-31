cmake_minimum_required(VERSION 3.10)
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

set(TEST_TARGET trex_tests)

project(${TEST_TARGET})
set(CMAKE_CXX_STANDARD 20)

add_executable(${TEST_TARGET}
    TestAtlas.cpp
    TestBitmapHelpers.cpp
    TestFont.cpp
    TestTextShaper.cpp
    TestCharset.cpp
)

# trex
add_subdirectory(../ trex)
target_link_libraries(${TEST_TARGET} trex)

# Google Test
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(${TEST_TARGET} gtest gtest_main gmock)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TEST_TARGET})

# Copy fonts from examples
add_custom_target(copy_fonts
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../examples/fonts ${CMAKE_CURRENT_BINARY_DIR}/fonts
)
add_dependencies(trex copy_fonts)
