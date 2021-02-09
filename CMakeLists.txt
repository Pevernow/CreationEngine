cmake_minimum_required(VERSION 3.15)

project(CreationEngine LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/deps/cmake/" ${CMAKE_MODULE_PATH})

find_package(SDL2 REQUIRED)
find_package(bgfx REQUIRED CONFIG)
find_package(LuaJIT REQUIRED)
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
    src/camera.cpp
    src/gui.cpp
    src/lua/mod.cpp
    deps/noise.cpp
    deps/ray.cpp
    deps/imgui/imgui.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/bgfx-imgui/imgui_impl_bgfx.cpp
    deps/sdl-imgui/imgui_impl_sdl.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE deps/ ${SDL2_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${LUA_LIBRARY}
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
