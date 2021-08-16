#ifndef _TYPEMANAGER_S_H_
#define _TYPEMANAGER_S_H_
#include "../typemanager.h"

class Blockmodel_s
{
public:
    Blockmodel_s(string name, uint16_t id, const char* texture_path[6]);
    string name;
    string texture_path[6];
    uint16_t id;
};

class TypeManager_s : public TypeManager
{
public:
    vector<Blockmodel_s> blockmodel;
    void registerNode(const char* name, const char* texture_path[6]);
};
#endif