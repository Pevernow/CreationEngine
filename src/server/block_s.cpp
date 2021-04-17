#include "block_s.h"

Blockmodel_s::Blockmodel_s(
    string name, uint16_t id, const char* texture_path[6])
{
    this->name = name;
    this->id = id;
    if (id != 0) {
        for (int i = 0; i < 6; i++) {
            this->texture_path[i] = texture_path[i];
        }
    } else {
        for (int i = 0; i < 6; i++) {
            this->texture_path[i] = "!empty";
        }
    }
}

void TypeManager_s::registerNode(const char* name, const char* texture_path[6])
{
    int id = blockmodel.size();
    nameIndex[name] = id;
    blockmodel.push_back(Blockmodel_s(name, id, texture_path));
}