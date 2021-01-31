#ifndef _WINDOW_H_
#define _WINDOW_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

class Renderer
{
public:
    ~Renderer();
    bool init(int width, int height);
    void shutdown();
    int width, height;
    SDL_Window* sdl_window;
};

#endif