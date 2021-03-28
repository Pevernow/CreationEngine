#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "block.h"
#include "camera.h"
#include "config.h"
#include "gui.h"
#include "lua/mod.h"
#include "window.h"
#include "world.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <map>

int FPS;

Luaenv luaenv;

GUImanager gui;

Renderer renderer;

World world;

Camera camera;

TypeManager typemanager;

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
    gui.init();

    GenBlockModel();

    luaenv.init();
    luaenv.execmods();

    camera.width = width;
    camera.height = height;

    world.generate_map();
    _FPS_Timer = SDL_GetTicks();
    // update loop
    while (!quit) {
        processEvent(renderer.sdl_window);

        camera.update_camera_position(SDL_GetTicks() - _FPS_Timer);

        gui.view();

        // FPS_limit
        if (SDL_GetTicks() - _FPS_Timer < max_frame_time) {
            SDL_Delay(max_frame_time - SDL_GetTicks() + _FPS_Timer);
        }
        FPS = 1000 / (SDL_GetTicks() - _FPS_Timer);
        _FPS_Timer = SDL_GetTicks();

        camera.view();

        Draw_blocks();

        bgfx::frame();
    }

    // shutdown
    luaenv.destory();
    renderer.shutdown();
    WriteConfig("CE.conf", config);
    SDL_ShowCursor(true);
    return 0;
}
