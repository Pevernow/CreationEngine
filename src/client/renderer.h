#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "../world.h"
#include "block_c.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>

class Renderer
{
public:
    ~Renderer();
    bool init(int width, int height, World* world, TypeManager_c* typemanager);
    void DrawBlock();
    void shutdown();
    int width, height;
    SDL_Window* sdl_window;

private:
    World* world;
    TypeManager_c* typemanager;
    bgfx::VertexBufferHandle block_vbh;
    bgfx::IndexBufferHandle block_ibh;
    bgfx::UniformHandle block_tex;
    bgfx::ProgramHandle program;
    bool GenBlockModel();
    void BlockDestroy();
};

#endif