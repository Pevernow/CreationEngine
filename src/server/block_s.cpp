#include "block_s.h"

Blockmodel_s::Blockmodel_s(string name, uint16_t id, const char* texture_path)
{
    this->name = name;
    this->id = id;
    if (id != 0) {
        this->texture_path = texture_path;
    } else {
        this->texture_path = "!empty";
    }
}

void TypeManager_s::registerNode(const char* name, const char* texture_path)
{
    int id = blockmodel.size();
    nameIndex[name] = id;
    blockmodel.push_back(Blockmodel_s(name, id, texture_path));
}