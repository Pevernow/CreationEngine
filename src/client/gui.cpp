#include "gui.h"

#include "../world.h"
#include "bgfx-imgui/imgui_impl_bgfx.h"
#include "imgui/imgui.h"
#include "utils.h"
#include <string>

void GUImanager::init(
    SDL_Window* sdl_window_ptr, int* FPS_ptr, glm::vec3* playerPos_ptr,
    string* pointThing_ptr, float* yaw_ptr, float* pitch_ptr,
    TypeManager_c* tm_ptr, Inventory* bag_ptr)
{
    sdl_window = sdl_window_ptr;
    FPS = FPS_ptr;
    yaw = yaw_ptr;
    pitch = pitch_ptr;
    playerPos = playerPos_ptr;
    pointThing = pointThing_ptr;
    tm = tm_ptr;
    bag = bag_ptr;

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
    cross = loadTexture("textures/plus.png");
    hotBarBg = loadTexture("textures/gui_hotbar.png");
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
}

void GUImanager::view()
{
    ImGui_Implbgfx_NewFrame();

    ImGui_ImplSDL2_NewFrame(sdl_window);
    ImGui::NewFrame();

    showdebuginfo();
    showHotbar();

    ImGui::Render();

    ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());

    return;
}

void GUImanager::showdebuginfo()
{
    ImGui::Begin(
        "Debug", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    std::string fps = "FPS:" + std::to_string(*FPS);
    std::string position = "Pos(" + std::to_string(playerPos->x) + ',' +
                           std::to_string(playerPos->y) + ',' +
                           std::to_string(playerPos->z) + ')';
    std::string dir =
        "Yaw:" + std::to_string(*yaw) + " Pitch : " + std::to_string(*pitch);
    std::string choose = "PointThing:" + *pointThing;
    ImGui::Text(choose.c_str());
    ImGui::Text(fps.c_str());

    ImGui::Text(position.c_str());
    ImGui::Text(dir.c_str());

    ImGui::Text("HP:");
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0, 0.0, 0.0, 1.0));
    ImGui::ProgressBar(0.9, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleColor(1);

    ImGui::End();
    ImGui::Begin(
        "Crosshair", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(256, 176), ImGuiCond_Always);
    ImGui::Image((void*)(long)(cross.idx), ImVec2(128, 128));
    ImGui::End();
}

void GUImanager::showHotbar()
{
    ImGui::Begin(
        "Hotbar", nullptr,
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowPos(ImVec2(70, 400), ImGuiCond_Always);
    ImGui::Image((void*)(long)(hotBarBg.idx), ImVec2(500, 65));
    for (int i = 0; i < 8; i++) {
        if (bag->items[i].id == "!empty") {
            continue;
        }
        ImGui::Image(
            (void*)(long)(tm->itemmodel[bag->items[i].id].texture.idx),
            ImVec2(128, 128));
        ImGui::Text(to_string(bag->items[i].num).c_str());
    }
    ImGui::End();
}