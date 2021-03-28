#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../config.h"
#include "../world.h"
#include "camera.h"
#include "gui.h"
#include "renderer.h"
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

class Client
{
public:
    void init(World* localserverworldptr, TypeManager* localTMptr);
    void mainloop();
    void shutdown();
    void on_left_click();
    void on_right_click();
    int FPS;

private:
    void processEvent(SDL_Window* window);
    map<string, string> config;
    GUImanager gui;
    Camera camera;
    Renderer renderer;
    int width, height;
    World* localworld;
    TypeManager* localTM;
    bool quit = false;
    int _FPS_Timer;
    int max_frame_time;
};

#endif