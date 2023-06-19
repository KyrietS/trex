cmake_minimum_required(VERSION 3.11)
include(FetchContent)

project(TrexExamples)

set(CMAKE_CXX_STANDARD 20)

function(add_example_project TARGET_NAME)
    add_executable(${TARGET_NAME} ${TARGET_NAME}.cpp)
    target_link_libraries(${TARGET_NAME} trex raylib)
endfunction()

add_example_project(Example_1_AtlasGeneration)
add_example_project(Example_2_LoadAllChars)
add_example_project(Example_3_RenderSingleCharacters)
add_example_project(Example_4_FontFromMemory)
add_example_project(Example_5_RenderFontSDF)
add_example_project(Example_6_TextShaping)
add_example_project(Example_7_MeasureText)

# Add Trex library
add_subdirectory(../ trexlib)
# Add Trex library with its dependencies to a folder
set_target_properties(trex PROPERTIES FOLDER "Trex")
# Get all the dependencies of "trex"
get_target_property(dependencies trex INTERFACE_LINK_LIBRARIES)
# Iterate over the dependencies and put them in "Trex" folder
foreach(dependency ${dependencies})
    set_target_properties(${dependency} PROPERTIES FOLDER "Trex")
endforeach()

# Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        4.5.0
)
FetchContent_MakeAvailable(raylib)
set_target_properties(raylib PROPERTIES FOLDER "Raylib")

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
