cmake_minimum_required(VERSION 3.15)

project(CreationEngine LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED CONFIG)
#find_package(imgui REQUIRED CONFIG)

add_executable(${PROJECT_NAME})
target_sources(
    ${PROJECT_NAME} PRIVATE
    src/main.cpp
    src/config.cpp
    src/utils.cpp
    src/block.cpp
    src/window.cpp
    src/world.cpp
    src/io.cpp
    src/camera.cpp
    src/mapgen.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE src/ ${SDL2_INCLUDE_DIRS})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    bgfx::bgfx
    ${SDL2_LIBRARIES}
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)

# required for project when using visual studio generator
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

if (WIN32)
# copy the SDL2.dll to the same folder as the executable
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2::SDL2>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
    VERBATIM)
endif()
