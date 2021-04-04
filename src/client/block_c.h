#ifndef _BLOCK_C_H_
#define _BLOCK_C_H_
#include "../block.h"

class Blockmodel_c
{
public:
    Blockmodel_c(string name, uint16_t id, const char* texture_path);
    string name;
    bgfx::TextureHandle textureData;
    uint16_t id;
};

class TypeManager_c : public TypeManager
{
public:
    vector<Blockmodel_c> blockmodel;
    void registerNode(const char* name, const char* texture_path);
};
#endif