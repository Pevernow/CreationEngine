#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "block.h"
#include "camera.h"
#include "config.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <map>

Renderer renderer;

map<string, bgfx::TextureHandle> Blockmodels;

Block world[10][10][10];

Camera camera;

map<string, string> config;

int main(int argc, char** argv)
{
    // init
    ReadConfig("CE.conf", config);
    PrintConfig(config);
    int max_frame_time = 1000 / 60;
    if (config.count("maxfps") == 1) {
        max_frame_time = 1000 / stoi(config["maxfps"]);
    }
    int _FPS_Timer = max_frame_time;

    const int width = stoi(config["width"]);
    const int height = stoi(config["height"]);

    if (renderer.init(width, height) == 1) {
        return 1;
    }

    camera.width = width;
    camera.height = height;

    Gen_block_model();
    // update loop
    for (bool quit = false; !quit;) {
        SDL_Event currentEvent;
        while (SDL_PollEvent(&currentEvent) != 0) {
            if (currentEvent.type == SDL_QUIT) {
                quit = true;
                break;
            }
        }

        camera.view();

        Draw_blocks();

        bgfx::frame();

        // FPS_limit
        if (SDL_GetTicks() - _FPS_Timer < max_frame_time) {
            SDL_Delay(max_frame_time - SDL_GetTicks() + _FPS_Timer);
        }
        _FPS_Timer = SDL_GetTicks();
    }

    // shutdown
    renderer.shutdown();
    WriteConfig("CE.conf", config);
    return 0;
}
