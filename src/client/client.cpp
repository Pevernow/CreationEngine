#include "client.h"
#include "../config.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "block_c.h"

#include <iostream>

void Client::init(World* localserverworldptr)
{
    localworld = localserverworldptr;
    // configure
    ReadConfig("CE.conf", config);
    PrintConfig(config);
    if (config.count("maxfps") == 1) {
        max_frame_time = 1000 / stoi(config["maxfps"]);
    }
    _FPS_Timer = max_frame_time;

    width = stoi(config["width"]);
    height = stoi(config["height"]);

    // init bgfx and sdl
    renderer.init(width, height, localworld, &localTM);
    camera.world = localworld;
    camera.tm = &localTM;

    localTM.init();

    gui.init(
        renderer.sdl_window, &FPS, &camera.position, &camera.pointThing,
        &camera.yaw, &camera.pitch);

    net.init(&localTM);

    net.eventloop();

    return;
}

void Client::shutdown()
{
    renderer.shutdown();
    return;
}

void Client::mainloop()
{
    net.startUp();
    _FPS_Timer = SDL_GetTicks();

    static Uint32 lastFrame;

    while (!quit) {
        Uint32 nowFrame = SDL_GetTicks();
        processEvent(renderer.sdl_window, nowFrame - lastFrame);

        camera.update_camera_position(nowFrame - lastFrame);

        gui.view();

        // FPS_limit
        if (nowFrame - lastFrame < max_frame_time) {
            SDL_Delay(max_frame_time - nowFrame + lastFrame);
        }
        if (nowFrame - _FPS_Timer >= 1000) {
            FPS = FPS_count;
            FPS_count = 0;
            _FPS_Timer = nowFrame;
        }
        lastFrame = nowFrame;
        FPS_count++;

        camera.view();

        renderer.DrawBlock();

        bgfx::frame();
    }
    shutdown();
}

void Client::processEvent(SDL_Window* window, int delay)
{
    // Event polling
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            quit = true;
        if (e.type == SDL_MOUSEMOTION) {
            camera.process_mouse_movement(
                renderer.width / 2 - e.motion.x,
                renderer.height / 2 - e.motion.y);
            SDL_WarpMouseInWindow(
                window, renderer.width / 2, renderer.height / 2);
        }
    }
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        camera.on_left_click(delay);
    } else {
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            camera.on_right_click(delay);
        }
    }
    // process move event
    const Uint8* state;
    state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_W]) {
        camera.processKeyboard(FORWARD, delay);
    }
    if (state[SDL_SCANCODE_S]) {
        camera.processKeyboard(BACKWARD, delay);
    }
    if (state[SDL_SCANCODE_A]) {
        camera.processKeyboard(LEFT, delay);
    }
    if (state[SDL_SCANCODE_D]) {
        camera.processKeyboard(RIGHT, delay);
    }

    if (state[SDL_SCANCODE_SPACE]) {
        camera.processKeyboard(JUMP, delay);
    }

    if (state[SDL_SCANCODE_ESCAPE]) {
        quit = true;
    }

    return;
}