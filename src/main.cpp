#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "block.h"
#include "camera.h"
#include "config.h"
#include "io.h"
#include "window.h"
#include "world.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <map>

Renderer renderer;

map<string, bgfx::TextureHandle> Blockmodels;

World world;

Camera camera;

map<string, string> config;

bool quit = false;

int _FPS_Timer;

int main(int argc, char** argv)
{
    // init
    ReadConfig("CE.conf", config);
    PrintConfig(config);
    int max_frame_time = 1000 / 60;
    if (config.count("maxfps") == 1) {
        max_frame_time = 1000 / stoi(config["maxfps"]);
    }
    _FPS_Timer = max_frame_time;

    const int width = stoi(config["width"]);
    const int height = stoi(config["height"]);

    if (renderer.init(width, height) == 1) {
        return 1;
    }

    camera.width = width;
    camera.height = height;

    Gen_block_model();
    world.generate_map();
    _FPS_Timer = SDL_GetTicks();
    // update loop
    while (!quit) {
        processEvent(renderer.sdl_window);

        camera.update_camera_position(SDL_GetTicks() - _FPS_Timer);

        // FPS_limit
        if (SDL_GetTicks() - _FPS_Timer < max_frame_time) {
            SDL_Delay(max_frame_time - SDL_GetTicks() + _FPS_Timer);
        }
        _FPS_Timer = SDL_GetTicks();

        camera.view();

        Draw_blocks();

        bgfx::frame();
    }

    // shutdown
    renderer.shutdown();
    WriteConfig("CE.conf", config);
    SDL_ShowCursor(true);
    return 0;
}
