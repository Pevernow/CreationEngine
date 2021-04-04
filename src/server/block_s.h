#ifndef _BLOCK_S_H_
#define _BLOCK_S_H_
#include "../block.h"

class Blockmodel_s
{
public:
    Blockmodel_s(string name, uint16_t id, const char* texture_path);
    string name;
    string texture_path;
    uint16_t id;
};

class TypeManager_s : public TypeManager
{
public:
    vector<Blockmodel_s> blockmodel;
    void registerNode(const char* name, const char* texture_path);
};
#endif