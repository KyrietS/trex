cmake_minimum_required(VERSION 3.5)

project(trex_examples)

set(CMAKE_CXX_STANDARD 20)

function(add_example_project TARGET_NAME)
    add_executable(${TARGET_NAME} ${TARGET_NAME}.cpp)
    target_link_libraries(${TARGET_NAME} trex raylib)
endfunction()

add_example_project(BasicAtlasGeneration)
add_example_project(LoadAllChars)

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
add_subdirectory(raylib)
set_target_properties(raylib PROPERTIES FOLDER "Raylib")

# Copy fonts to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
