#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <bgfx/bgfx.h>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Block
{
public:
    int16_t x;
    int16_t y;
    int16_t z;
    uint16_t id=0;
    bool show;
};

class Blockmodel
{
public:
    Blockmodel(string name, uint16_t id, const char* texture_path);
    string name;
    bgfx::TextureHandle textureData;
    uint16_t id;
};

class TypeManager
{
public:
    vector<Blockmodel> blockmodel;
    void registerNode(const char* name, const char* texture_path);
    int nameToID(string name); // use id to index map of blockmodels.
private:
    map<string, int> nameIndex;
};

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;
    int16_t u;
    int16_t v;
};

bool GenBlockModel();

void Block_destroy();

void Draw_blocks();

#endif