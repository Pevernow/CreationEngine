cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

project(CreationEngine LANGUAGES CXX)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/deps/cmake/" ${CMAKE_MODULE_PATH})
#set(CMAKE_CXX_FLAGS -pg)

find_package(SDL2 REQUIRED CONFIG)
find_package(bgfx REQUIRED CONFIG)
find_package(LuaJIT REQUIRED)


add_executable(${PROJECT_NAME})

#Include asio headers
set(ASIO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/deps/asio/include)
#include_directories(${ASIO_INCLUDE})
#message(${ASIO_INCLUDE})
target_compile_options(CreationEngine PUBLIC -DASIO_STANDALONE)
#set(CMAKE_CXX_FLAGS -DASIO_STANDALONE)
target_include_directories(CreationEngine PUBLIC ${ASIO_INCLUDE})
if (WIN32)
target_link_libraries(CreationEngine PUBLIC -lwsock32 -lws2_32)
endif()

#Complie asio_kcp
target_include_directories(CreationEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/client_lib ${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/server_lib)
file(GLOB_RECURSE asio_kcp_src 
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/client_lib/*.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/server_lib/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/server_lib/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/essential/_src/check_function.cpp
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/util/connect_packet.cpp
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/util/ikcp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/deps/asio_kcp/essential/_src/utility/strutil.cpp
)

target_include_directories(CreationEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/spdlog/)

target_sources(
    ${PROJECT_NAME} PRIVATE
    
    src/main.cpp
    src/config.cpp
    src/client/utils.cpp
    src/typemanager.cpp
    src/client/client.cpp
    src/client/renderer.cpp
    src/world.cpp
    src/client/player_c.cpp
    src/client/gui.cpp
    src/client/typemanager_c.cpp
    src/client/network_c.cpp
    src/server/lua/mod.cpp
    src/server/server.cpp
    src/server/world_s.cpp
    src/server/typemanager_s.cpp
    src/server/network_s.cpp
    deps/noise.cpp
    deps/ray.cpp
    deps/imgui/imgui.cpp
    deps/imgui/imgui_widgets.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_tables.cpp
    deps/bgfx-imgui/imgui_impl_bgfx.cpp
    deps/sdl-imgui/imgui_impl_sdl.cpp
    ${asio_kcp_src}
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
