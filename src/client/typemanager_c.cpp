#include "typemanager_c.h"
#include "utils.h"
#include <cstring>

void TypeManager_c::init()
{
    textureCount = 0;
    textureArray = bgfx::createTexture2D(
        16, 16, false, 30, bgfx::TextureFormat::RGBA8,
        BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT);
}

Blockmodel_c::Blockmodel_c(
    string name, uint16_t id, const char* texture_path[6])
{
    this->name = name;
    this->id = id;
}

Itemmodel_c::Itemmodel_c(const char* name, const char* inventory_image)
{
    this->name = string(name);
    if (strcmp(inventory_image, "!empty") != 0) {
        this->texture = loadTexture(inventory_image);
    }
}

void TypeManager_c::registerNode(const char* name, const char* texture_path[6])
{
    int id = blockmodel.size();
    nameIndex[name] = id;
    if (strcmp(texture_path[0], "!empty") != 0) {
        loadBlockTexture(
            textureArray, textureCount, texture_path[0], texture_path[1],
            texture_path[2], texture_path[3], texture_path[4], texture_path[5]);
        textureCount += 6;

        bgfx::TextureHandle ici = createInventoryCubeImage(
            texture_path[0], texture_path[4], texture_path[5]);

        // maunal registerItem
        auto item = Itemmodel_c();
        item.name = name;
        item.texture = ici;
        itemmodel[string(name)] = item;
    }
    blockmodel.push_back(Blockmodel_c(name, id, texture_path));
}

void TypeManager_c::registerItem(const char* name, const char* texture_path)
{
    itemmodel[string(name)] = Itemmodel_c(name, texture_path);
}