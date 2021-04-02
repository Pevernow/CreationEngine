#include "block_c.h"
#include "../utils.h"

Blockmodel_c::Blockmodel_c(string name, uint16_t id, const char* texture_path)
{
    this->name = name;
    this->id = id;
    if (id != 0)
        textureData = loadTexture(texture_path);
}

void TypeManager_c::registerNode(const char* name, const char* texture_path)
{
    int id = blockmodel.size();
    nameIndex[name] = id;
    blockmodel.push_back(Blockmodel_c(name, id, texture_path));
}