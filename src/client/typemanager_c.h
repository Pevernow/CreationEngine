#ifndef _BLOCK_C_H_
#define _BLOCK_C_H_
#include "../typemanager.h"

class Blockmodel_c
{
public:
    Blockmodel_c(string name, uint16_t id, const char* texture_path[6]);
    string name;
    int textureDataId[6];
    bgfx::TextureHandle textureData;
    uint16_t id;
};

class Itemmodel_c
{
public:
    Itemmodel_c(){};
    Itemmodel_c(const char* name, const char* inventory_image);
    string name;
    bgfx::TextureHandle texture;
};

class TypeManager_c : public TypeManager
{
public:
    void init();
    vector<Blockmodel_c> blockmodel;

    map<string, Itemmodel_c> itemmodel;
    bgfx::TextureHandle textureArray;
    void registerNode(const char* name, const char* texture_path[6]);
    void registerItem(const char* name, const char* texture_path);

private:
    int textureCount;
};
#endif