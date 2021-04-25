#include "bgfx/bgfx.h"
#include "glm/glm.hpp"
#include "sdl-imgui/imgui_impl_sdl.h"
#include <string>

class GUImanager
{
public:
    void init(
        SDL_Window* sdl_window_ptr, int* FPS_ptr, glm::vec3* playerPos_ptr,
        std::string* pointThing_ptr, float* yaw_ptr, float* pitch_ptr);
    void view();
    void showdebuginfo();

private:
    SDL_Window* sdl_window;
    bgfx::TextureHandle cross;
    glm::vec3* playerPos;
    std::string* pointThing;
    float *yaw, *pitch;
    int* FPS;
};