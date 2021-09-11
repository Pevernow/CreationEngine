#include "renderer.h"

#include "../file-ops.h"
#include "bgfx/bgfx.h"
#include "bgfx/platform.h"
#include "typemanager_c.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <bx/math.h>

float vertices[] = {
    // Back face
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, // Bottom-left
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 3.0f, // top-right
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f, // bottom-right
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 3.0f, // top-right
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f, // bottom-left
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 3.0f, // top-left
    // Front face
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, // bottom-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f, // bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, // top-right
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, // top-right
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 2.0f, // top-left
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, // bottom-left
    // Left face
    0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 4.0f, // top-right
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 4.0f, // top-left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f, // bottom-left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f, // bottom-left
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f, // bottom-right
    0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 4.0f, // top-right
    // Right face
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 5.0f, // top-left
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 5.0f, // bottom-right
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 5.0f, // top-right
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 5.0f, // bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 5.0f, // top-left
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f, // bottom-left
    // Bottom face
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom-left
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
    // Top face
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // top-right
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom-right
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left
};

static const uint16_t s_cubeIndices[36] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11,

    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,

    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
};

static bgfx::ShaderHandle createShader(
    const std::string& shader, const char* name)
{
    const bgfx::Memory* mem = bgfx::copy(shader.data(), shader.size());
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);
    return handle;
}

bool Renderer::GenBlockModel()
{
    bgfx::VertexLayout pos_col_vert_layout;
    pos_col_vert_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 3, bgfx::AttribType::Float, true, true)
        .end();
    // 创建顶点缓冲对象
    block_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices, sizeof(vertices)), pos_col_vert_layout);
    // 创建索引缓冲对象
    block_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices)));
    block_tex = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

    std::string vshader;
    if (!fileops::read_file("shader/block/v_simple.bin", vshader)) {
        return 1;
    }
    std::string fshader;
    if (!fileops::read_file("shader/block/f_simple.bin", fshader)) {
        return 1;
    }
    bgfx::ShaderHandle vsh = createShader(vshader, "vshader");
    bgfx::ShaderHandle fsh = createShader(fshader, "fshader");

    program = bgfx::createProgram(vsh, fsh, true);
    return 0;
}

bool Renderer::init(
    int width, int height, World* worldptr, TypeManager_c* typemanagerptr)
{
    world = worldptr;
    typemanager = typemanagerptr;
    this->width = width;
    this->height = height;
    cache = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    sdl_window = SDL_CreateWindow(
        "CreationEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN);

    if (sdl_window == nullptr) {
        printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(sdl_window, &wmi)) {
        printf(
            "SDL_SysWMinfo could not be retrieved. SDL_Error: %s\n",
            SDL_GetError());
        return 1;
    }

    bgfx::renderFrame(); // single threaded mode
    bgfx::PlatformData pd{};
#if BX_PLATFORM_WINDOWS
    pd.nwh = wmi.info.win.window;
#endif
#if BX_PLATFORM_OSX
    pd.nwh = wmi.info.cocoa.window;
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX
#if BX_PLATFORM_LINUX
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#endif // BX_PLATFORM_WINDOWS ? BX_PLATFORM_OSX ? BX_PLATFORM_LINUX
    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::Count; // auto choose renderer
    bgfx_init.resolution.width = width;
    bgfx_init.resolution.height = height;
    bgfx_init.platformData = pd;
    bgfx::init(bgfx_init);

    bgfx::setViewClear(
        0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x6495EDFF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);

    GenBlockModel();
    sky_tex = loadTextureRAW("textures/sky.png")->m_data;
    SDL_ShowCursor(SDL_DISABLE);
    return 0;
}

void Renderer::makeDrawCache()
{
    // Calc Light Offset by time
    double SkyLightOffset = 0;
    if (world->time <= 4.5 * 60) { // Before 4.5am
        SkyLightOffset = -7;
    } else if (world->time > 4.5 * 60 && world->time <= 8 * 60) { // Morning
        SkyLightOffset = max(-7 + (world->time - 4.5 * 60) * 0.034, 0.0);
    } else if (world->time > 8 * 60 && world->time <= 17 * 60) {
        SkyLightOffset = 0;
    } else if (world->time > 17 * 60 && world->time <= 19 * 60) {
        SkyLightOffset = min(0 - (world->time - 17 * 60) * 0.06, -7.0);
    } else if (world->time > 19 * 60) {
        SkyLightOffset = -7;
    }

    int tmSize = typemanager->blockmodel.size();

    vector<Block*> renderList;
    vector<int8_t> light_around;
    for (int i = 0, l = world->worldmap.size(); i < l; i++) {
        Chunk& chunk = world->worldmap[i];
        if (chunk.show == false) {
            continue;
        }
        Block& mblock = chunk.blocks[0][0][0];
        int mx = mblock.x;
        int my = mblock.y;
        int mz = mblock.z;
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    if (chunk.blocks[x][y][z].show == true &&
                        tmSize > chunk.blocks[x][y][z].id) {
                        renderList.emplace_back(&chunk.blocks[x][y][z]);

                        // TODO:smooth light
                        /*
                        Block* testBlock;
                        testBlock = ck.getBlockOrNullptr(x + 1, y, z);
                        if (testBlock == nullptr) {
                            light_around.emplace_back(0);
                        } else {
                            light_around.emplace_back(testBlock->sun_light);
                        }
                        testBlock = ck.getBlockOrNullptr(x - 1, y, z);
                        if (testBlock == nullptr) {
                            light_around.emplace_back(0);
                        } else {
                            light_around.emplace_back(testBlock->sun_light);
                        }
                        testBlock = ck.getBlockOrNullptr(x, y + 1, z);
                        if (testBlock == nullptr) {
                            light_around.emplace_back(0);
                        } else {
                            light_around.emplace_back(testBlock->sun_light);
                        }
                        testBlock = ck.getBlockOrNullptr(x, y - 1, z);
                        if (testBlock == nullptr) {
                            light_around.emplace_back(0);
                        } else {
                            light_around.emplace_back(testBlock->sun_light);
                        }
                        testBlock = ck.getBlockOrNullptr(x, y, z + 1);
                        if (testBlock == nullptr) {
                            light_around.emplace_back(0);
                        } else {
                            light_around.emplace_back(testBlock->sun_light);
                        }
                        testBlock = ck.getBlockOrNullptr(x, y, z - 1);
                        if (testBlock == nullptr) {
                            light_around.emplace_back(0);
                        } else {
                            light_around.emplace_back(testBlock->sun_light);
                        }
                        */
                    }
                }
            }
        }
    }

    // Start Instancing
    uint16_t instanceStride = 64 + 16;
    bgfx::allocInstanceDataBuffer(&idb, renderList.size(), instanceStride);

    uint8_t* data = idb.data;
    for (int i = 0, l = renderList.size(); i < l; i++) {
        Block* block = renderList[i];
        float* mtx = (float*)data;
        bx::mtxTranslate(mtx, block->x, block->y, block->z);
        float* id = (float*)&data[64];
        id[0] = block->id - 1;
        id[1] = max(min(block->sun_light + (int)SkyLightOffset, 15), 0);
        id[2] = 0;
        id[3] = 0;
        /*
        id[2] = light_around[i * 6 + 0];
        id[3] = light_around[i * 6 + 1];
        id[4] = light_around[i * 6 + 2];
        id[5] = light_around[i * 6 + 3];
        id[6] = light_around[i * 6 + 4];
        id[7] = light_around[i * 6 + 5];
        */
        data += instanceStride;
    }
}

void Renderer::DrawBlock()
{
    if (cache == true) {
        makeDrawCache();
        cache = false;
    }

    // Set vertex and index buffer.
    bgfx::setTexture(0, block_tex, typemanager->textureArray);
    bgfx::setVertexBuffer(0, block_vbh);
    bgfx::setIndexBuffer(block_ibh);

    // Set instance data buffer.
    bgfx::setInstanceDataBuffer(&idb);

    // Set render states.
    bgfx::setState(
        0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA | BGFX_STATE_CULL_CCW |
        BGFX_STATE_BLEND_ALPHA);
    // Submit primitive for rendering to view 0.
    bgfx::submit(0, program);
}

void Renderer::BlockDestroy()
{
    bgfx::destroy(block_ibh);
    bgfx::destroy(block_vbh);
    bgfx::destroy(program);
}

void Renderer::shutdown()
{
    BlockDestroy();
    bgfx::shutdown();
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

Renderer::~Renderer()
{
    ;
}

void Renderer::RenderSky()
{
    uint32_t col =
        ((uint32_t*)&sky_tex)[max(int(float(world->time) / 1440 * 1024), 1023)];
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, col, 1.0f, 0);
}