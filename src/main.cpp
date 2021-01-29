#include "Block.h"
#include "Config.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "bx/math.h"
#include <map>

map<string, bgfx::TextureHandle> Blockmodels;

Block world[10][10][20];

map<string, string> config;

int main(int argc, char** argv)
{
    // init
    ReadConfig("CE.conf", config);
    PrintConfig(config);
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    const int width = stoi(config["width"]);
    const int height = stoi(config["height"]);
    SDL_Window* window = SDL_CreateWindow(
        argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
        height, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        printf(
            "SDL_SysWMinfo could not be retrieved. SDL_Error: %s\n",
            SDL_GetError());
        return 1;
    }
    bgfx::renderFrame(); // single threaded mode
    bgfx::PlatformData pd{};
#if BX_PLATFORM_WINDOWS
    pd.nwh = wmi.info.win.window;
#endif
#if BX_PLATFORM_OSX
    pd.nwh = wmi.info.cocoa.window;
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX
#if BX_PLATFORM_LINUX
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX

    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
    bgfx_init.resolution.width = width;
    bgfx_init.resolution.height = height;
    bgfx_init.platformData = pd;
    bgfx::init(bgfx_init);


    bgfx::setViewClear(
        0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);

    float cam_pitch = 0.0f;
    float cam_yaw = 0.0f;
    float rot_scale = 0.01f;

    int prev_mouse_x = 0;
    int prev_mouse_y = 0;


    Gen_block_model();
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    // update loop
    for (bool quit = false; !quit;) {
        SDL_Event currentEvent;
        while (SDL_PollEvent(&currentEvent) != 0) {
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
                break;
            }
        }

        // simple input code for orbit camera
        int mouse_x, mouse_y;
        const int buttons = SDL_GetGlobalMouseState(&mouse_x, &mouse_y);
        if ((buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0) {
            int delta_x = mouse_x - prev_mouse_x;
            int delta_y = mouse_y - prev_mouse_y;
            cam_yaw += float(-delta_x) * rot_scale;
            cam_pitch += float(-delta_y) * rot_scale;
        }

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;

        float cam_rotation[16];
        bx::mtxRotateXYZ(cam_rotation, cam_pitch, cam_yaw, 0.0f);

        float cam_translation[16];
        bx::mtxTranslate(cam_translation, 0.0f, 0.0f, -12.0f);

        float cam_transform[16];
        bx::mtxMul(cam_transform, cam_translation, cam_rotation);

        float view[16];
        bx::mtxInverse(view, cam_transform);

        float proj[16];
        bx::mtxProj(
            proj, 60.0f, float(width) / float(height), 0.1f, 100.0f,
            bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewTransform(0, view, proj);

        Draw_blocks();

        bgfx::frame();
    }

    // shutdown
    destroy();
    bgfx::shutdown();

    SDL_DestroyWindow(window);
    SDL_Quit();
    WriteConfig("CE.conf", config);
    return 0;
}
