cmake_minimum_required(VERSION 3.15)

project(CreationEngine LANGUAGES CXX)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/deps/cmake/" ${CMAKE_MODULE_PATH})
#set(CMAKE_CXX_FLAGS -pg)

find_package(SDL2 REQUIRED CONFIG)
find_package(bgfx REQUIRED CONFIG)
find_package(LuaJIT REQUIRED)
find_package(Libevent REQUIRED CONFIG)

add_executable(${PROJECT_NAME})
target_sources(
    ${PROJECT_NAME} PRIVATE
    src/main.cpp
    src/config.cpp
    src/client/utils.cpp
    src/block.cpp
    src/client/client.cpp
    src/client/renderer.cpp
    src/world.cpp
    src/client/camera.cpp
    src/client/gui.cpp
    src/client/block_c.cpp
    src/client/network_c.cpp
    src/server/lua/mod.cpp
    src/server/server.cpp
    src/server/world_s.cpp
    src/server/block_s.cpp
    src/server/network_s.cpp
    deps/noise.cpp
    deps/ray.cpp
    deps/imgui/imgui.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/bgfx-imgui/imgui_impl_bgfx.cpp
    deps/sdl-imgui/imgui_impl_sdl.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE deps/ ${SDL2_INCLUDE_DIRS} ${LUA_INCLUDE_DIR} {LIBEVENT_INCLUDE_DIR})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${LUA_LIBRARY}
    ${LIBEVENT_LIBRARY}
    bgfx::bgfx
    ${SDL2_LIBRARIES}
    ${}
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
