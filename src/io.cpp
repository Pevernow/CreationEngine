#include "io.h"
#include "camera.h"
#include "window.h"

extern bool quit;
extern int _FPS_Timer;
extern Camera camera;

void processEvent(SDL_Window* window)
{
    // Event polling
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        // If user closes he window
        if (e.type == SDL_QUIT)
            quit = true;
        // If user presses any key
        /*
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_w:
                    camera.processKeyboard(
                        FORWARD, SDL_GetTicks() - _FPS_Timer);
                    break;

                case SDLK_s:
                    camera.processKeyboard(
                        BACKWARD, SDL_GetTicks() - _FPS_Timer);
                    break;

                case SDLK_a:
                    camera.processKeyboard(LEFT, SDL_GetTicks() - _FPS_Timer);
                    break;

                case SDLK_d:
                    camera.processKeyboard(RIGHT, SDL_GetTicks() - _FPS_Timer);
                    break;

                case SDLK_SPACE:
                    camera.processKeyboard(JUMP, SDL_GetTicks() - _FPS_Timer);
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
        */
        if (e.type == SDL_MOUSEMOTION) {
            camera.process_mouse_movement(
                e.motion.x - camera.width / 2, camera.height / 2 - e.motion.y);
            // reversed since y-coordinates go from bottom to top
            SDL_WarpMouseInWindow(window, camera.width / 2, camera.height / 2);
        }
        // If user clicks the mouse
        // if (e.type == SDL_MOUSEBUTTONDOWN)
        // quit = true;
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

    return;
}