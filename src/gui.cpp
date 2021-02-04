#include "gui.h"

#include "bgfx-imgui/imgui_impl_bgfx.h"
#include "camera.h"
#include "imgui/imgui.h"
#include "sdl-imgui/imgui_impl_sdl.h"
#include "window.h"
#include "world.h"
#include <string>

extern Renderer renderer;
extern Camera camera;
extern int FPS;

void GUImanager::init()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_Implbgfx_Init(255);
#if BX_PLATFORM_WINDOWS
    ImGui_ImplSDL2_InitForD3D(sdl_window);
#elif BX_PLATFORM_OSX
    ImGui_ImplSDL2_InitForMetal(sdl_window);
#elif BX_PLATFORM_LINUX
    ImGui_ImplSDL2_InitForOpenGL(sdl_window, nullptr);
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX
}

void GUImanager::view()
{
    ImGui_Implbgfx_NewFrame();
    ImGui_ImplSDL2_NewFrame(renderer.sdl_window);
    ImGui::NewFrame();

    showdebuginfo();

    ImGui::Render();

    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

    return;
}

void GUImanager::showdebuginfo()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    std::string fps = "FPS:" + std::to_string(FPS);
    std::string position = "Pos(" + std::to_string(camera.position.x) + ',' +
                           std::to_string(camera.position.y) + ',' +
                           std::to_string(camera.position.z) + ')';
    std::string dir = "Yaw:" + std::to_string(camera.yaw) +
                      " Pitch : " + std::to_string(camera.pitch);
    std::string choose = "Choose(" + std::to_string(camera.choosepos.x) + ',' +
                         std::to_string(camera.choosepos.y) + ',' +
                         std::to_string(camera.choosepos.z) + ')';
    ImGui::Text(choose.c_str());
    ImGui::Text(fps.c_str());
    ImGui::Text(position.c_str());
    ImGui::Text(dir.c_str());
    ImGui::End();
    return;
}