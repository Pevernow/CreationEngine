#include "block_c.h"
#include "utils.h"
#include <cstring>

Blockmodel_c::Blockmodel_c(
    string name, uint16_t id, const char* texture_path[6])
{
    this->name = name;
    this->id = id;
    if (strcmp(texture_path[0], "!empty") != 0) {
        textureData = loadBlockTexture(
            texture_path[0], texture_path[1], texture_path[2], texture_path[3],
            texture_path[4], texture_path[5]);
    }
}

void TypeManager_c::registerNode(const char* name, const char* texture_path[6])
{
    int id = blockmodel.size();
    nameIndex[name] = id;
    blockmodel.push_back(Blockmodel_c(name, id, texture_path));
}