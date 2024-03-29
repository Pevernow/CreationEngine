#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "../world.h"
#include "typemanager_c.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>

class Renderer
{
public:
    ~Renderer();
    bool init(int width, int height, World* world, TypeManager_c* typemanager);
    void DrawBlock();
    void RenderSky();
    void shutdown();
    void makeDrawCache();
    bool cache;
    int width, height;
    SDL_Window* sdl_window;

private:
    void* sky_tex;
    World* world;
    TypeManager_c* typemanager;
    bgfx::VertexBufferHandle block_vbh;
    bgfx::IndexBufferHandle block_ibh;
    bgfx::UniformHandle block_tex;
    bgfx::ProgramHandle program;
    bgfx::InstanceDataBuffer idb;
    bool GenBlockModel();
    void BlockDestroy();
};

#endif