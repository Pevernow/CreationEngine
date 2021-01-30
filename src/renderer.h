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

class Camera
{
public:
    Camera();
    void view();
    float x, y, z;
    float yaw, pitch, scale;
    int prev_mouse_x, prev_mouse_y;
    float width, height;
};

#endif