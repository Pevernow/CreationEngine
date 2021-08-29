#include "bgfx/bgfx.h"
#include "glm/glm.hpp"
#include "sdl-imgui/imgui_impl_sdl.h"
#include "typemanager_c.h"
#include <string>

class GUImanager
{
public:
    void init(
        SDL_Window* sdl_window_ptr, int* FPS_ptr, glm::vec3* playerPos_ptr,
        std::string* pointThing_ptr, float* yaw_ptr, float* pitch_ptr,
        TypeManager_c* tm_ptr, Inventory* bag_ptr, int* wielditem_ptr);
    void view();

private:
    void showdebuginfo();
    void showHotbar();

private:
    TypeManager_c* tm;
    SDL_Window* sdl_window;
    bgfx::TextureHandle cross, hotBarBg, hotBarSelected;
    const glm::vec3* playerPos;
    const std::string* pointThing;
    const float *yaw, *pitch;
    const int* FPS;
    const Inventory* bag; // Just use for hot bar
    const int* wielditem;
};