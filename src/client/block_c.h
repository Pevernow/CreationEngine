#ifndef _BLOCK_C_H_
#define _BLOCK_C_H_
#include "../block.h"

class Blockmodel_c
{
public:
    Blockmodel_c(string name, uint16_t id, const char* texture_path[6]);
    string name;
    int textureDataId[6];
    bgfx::TextureHandle textureData;
    uint16_t id;
};

class TypeManager_c : public TypeManager
{
public:
    void init();
    vector<Blockmodel_c> blockmodel;
    bgfx::TextureHandle textureArray;
    void registerNode(const char* name, const char* texture_path[6]);

private:
    int textureCount;
};
#endif