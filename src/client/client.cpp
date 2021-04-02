#include "client.h"
#include "../config.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "block_c.h"

void Client::init(World* localserverworldptr, TypeManager_c* localTMptr)
{
    localTM = localTMptr;
    localworld = localserverworldptr;
    // configure
    ReadConfig("CE.conf", config);
    PrintConfig(config);
    if (config.count("maxfps") == 1) {
        int max_frame_time = 1000 / stoi(config["maxfps"]);
    }
    _FPS_Timer = max_frame_time;

    width = stoi(config["width"]);
    height = stoi(config["height"]);

    // init bgfx and sdl
    renderer.init(width, height, localworld, localTM);
    camera.world = localworld;

    gui.init(renderer.sdl_window);

    return;
}

void Client::shutdown()
{
    renderer.shutdown();
    return;
}

void Client::mainloop()
{
    _FPS_Timer = SDL_GetTicks();

    while (!quit) {
        processEvent(renderer.sdl_window);

        camera.update_camera_position(SDL_GetTicks() - _FPS_Timer);

        gui.view();

        // FPS_limit
        if (SDL_GetTicks() - _FPS_Timer < max_frame_time) {
            SDL_Delay(max_frame_time - SDL_GetTicks() + _FPS_Timer);
        }
        if (SDL_GetTicks() - _FPS_Timer == 0) {
            FPS = 999;
        } else {
            FPS = 1000 / (SDL_GetTicks() - _FPS_Timer);
        }
        _FPS_Timer = SDL_GetTicks();

        camera.view();

        renderer.DrawBlock();

        bgfx::frame();
    }
    shutdown();
}

void Client::processEvent(SDL_Window* window)
{
    // Event polling
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            quit = true;
        if (e.type == SDL_MOUSEMOTION) {
            camera.process_mouse_movement(
                e.motion.x - renderer.width / 2,
                renderer.height / 2 - e.motion.y);
            SDL_WarpMouseInWindow(
                window, renderer.width / 2, renderer.height / 2);
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