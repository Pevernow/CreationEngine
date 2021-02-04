#include "block.h"
#include "file-ops.h"
#include "utils.h"
#include "world.h"
#include <bx/math.h>
#include <iostream>
#include <map>

extern map<string, bgfx::TextureHandle> Blockmodels;
extern World world;

bgfx::VertexBufferHandle block_vbh;
bgfx::IndexBufferHandle block_ibh;
bgfx::UniformHandle block_tex;
bgfx::ProgramHandle program;

float vertices[] = {
    // Back face
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    // Front face
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top-right
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top-right
    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // top-left
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    // Left face
    0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top-right
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-right
    0.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top-right
                                  // Right face
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top-left
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
    1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // top-left
    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    // Bottom face
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-right
    1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-left
    1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-left
    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-right
    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top-right
    // Top face
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // bottom-right
    0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    0.0f, 1.0f, 1.0f, 0.0f, 0.0f  // bottom-left
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

bool Gen_block_model()
{
    bgfx::VertexLayout pos_col_vert_layout;
    pos_col_vert_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true, true)
        .end();
    // 创建顶点缓冲对象
    block_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices, sizeof(vertices)), pos_col_vert_layout);
    // 创建索引缓冲对象
    block_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices)));
    block_tex = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

    std::string vshader;
    if (!fileops::read_file("shader/v_simple.bin", vshader)) {
        return 1;
    }
    std::string fshader;
    if (!fileops::read_file("shader/f_simple.bin", fshader)) {
        return 1;
    }
    bgfx::ShaderHandle vsh = createShader(vshader, "vshader");
    bgfx::ShaderHandle fsh = createShader(fshader, "fshader");

    program = bgfx::createProgram(vsh, fsh, true);

    // Blockmodels["fieldstone"] = loadTexture("textures/aeq62-oqx7h.dds");
    Blockmodels["default_dirt"] = loadTexture("textures/default_dirt.png");
    Blockmodels["blue_wool"] = loadTexture("textures/mcl_wool_light_blue.png");
    return 0;
}

void destroy()
{
    bgfx::destroy(block_ibh);
    bgfx::destroy(block_vbh);
    bgfx::destroy(program);
}

bool is_type_registed(string type)
{
    if (type != "air" && Blockmodels.find(type) == Blockmodels.end()) {
        return false;
    }
    return true;
}

void Draw_blocks()
{
    for (int i = 0, l = world.worldmap.size(); i < l; i++) {
        Block mblock = world.worldmap[i].blocks[0][0][0];
        int mx = mblock.x;
        int my = mblock.y;
        int mz = mblock.z;
        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    if (world.worldmap[i].blocks[x][y][z].type != "air") {
                        float mtx[16];
                        bx::mtxTranslate(mtx, mx + x, my + y, mz + z);
                        bgfx::setTransform(mtx);
                        bgfx::setVertexBuffer(0, block_vbh);
                        bgfx::setIndexBuffer(block_ibh);
                        bgfx::setTexture(
                            0, block_tex,
                            Blockmodels
                                [world.worldmap[i].blocks[x][y][z].type]);
                        bgfx::setState(
                            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                            BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS |
                            BGFX_STATE_MSAA | BGFX_STATE_CULL_CCW);
                        bgfx::submit(0, program);
                    }
                }
            }
        }
    }
}