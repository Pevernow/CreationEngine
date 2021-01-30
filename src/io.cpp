#include"io.h"

extern bool quit;

void processEvent(SDL_Window* window)
{
    //Event polling
    while (SDL_PollEvent(&e))
    {
        //If user closes he window
        if (e.type == SDL_QUIT)
            quit = true;
        //If user presses any key
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                camera.ProcessKeyboard(FORWARD, deltaTime);
                break;

            case SDLK_s:
                camera.ProcessKeyboard(BACKWARD, deltaTime);
                break;

            case SDLK_a:
                camera.ProcessKeyboard(LEFT, deltaTime);
                break;

            case SDLK_d:
                camera.ProcessKeyboard(RIGHT, deltaTime);
                break;
            
            case SDLK_SPACE:
                camera.ProcessKeyboard(JUMP, deltaTime);
                break;
            case SDLK_ESCAPE:
                quit = true;
                break;
            default:
                cout << e.key.keysym.sym;
                break;
            }
        }
        if (e.type == SDL_WINDOWEVENT)
        {
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                glViewport(0, 0, e.window.data1, e.window.data2);
                break;
            }
        }
        if (e.type == SDL_MOUSEMOTION)
        {
            camera.ProcessMouseMovement(e.motion.x-SCREEN_WIDTH/2,SCREEN_HEIGHT/2-e.motion.y);
            // reversed since y-coordinates go from bottom to top
            SDL_WarpMouseInWindow(window,SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
        }
        //If user clicks the mouse
        //if (e.type == SDL_MOUSEBUTTONDOWN)
        //quit = true;
    }
    return;
}