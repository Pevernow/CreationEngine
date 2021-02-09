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

map<string, string> config;

bool quit = false;

int _FPS_Timer;

void processEvent(SDL_Window* window)
{
    // Event polling
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            quit = true;
        if (e.type == SDL_MOUSEMOTION) {
            camera.process_mouse_movement(
                e.motion.x - camera.width / 2, camera.height / 2 - e.motion.y);
            SDL_WarpMouseInWindow(window, camera.width / 2, camera.height / 2);
        }
        if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                camera.on_left_click();
            }
            if (e.button.button == SDL_BUTTON_RIGHT) {
                camera.on_right_click();
            }
        }
    }

    // process move event
    const Uint8* state;
    state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) {
        camera.processKeyboard(FORWARD, SDL_GetTicks() - _FPS_Timer);
    }
    if (state[SDL_SCANCODE_S]) {
        camera.processKeyboard(BACKWARD, SDL_GetTicks() - _FPS_Timer);
    }
    if (state[SDL_SCANCODE_A]) {
        camera.processKeyboard(LEFT, SDL_GetTicks() - _FPS_Timer);
    }
    if (state[SDL_SCANCODE_D]) {
        camera.processKeyboard(RIGHT, SDL_GetTicks() - _FPS_Timer);
    }

    if (state[SDL_SCANCODE_SPACE]) {
        camera.processKeyboard(JUMP, SDL_GetTicks() - _FPS_Timer);
    }

    if (state[SDL_SCANCODE_ESCAPE]) {
        quit = true;
    }

    return;
}

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

    luaenv.init();
    luaenv.execmods();

    camera.width = width;
    camera.height = height;

    Gen_block_model();
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
