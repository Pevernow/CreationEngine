#ifndef _TYPEMANAGER_S_H_
#define _TYPEMANAGER_S_H_
#include "../typemanager.h"

#include <map>
#include <vector>

class Blockmodel_s
{
public:
    Blockmodel_s(string name, uint16_t id, const char* texture_path[6]);
    string name;
    string texture_path[6];
    uint16_t id;
};

class Itemmodel_s
{
public:
    Itemmodel_s(){};
    Itemmodel_s(const char* name, const char* inventory_image);
    string name;
    string texture_path;
};

class TypeManager_s : public TypeManager
{
public:
    vector<Blockmodel_s> blockmodel;
    map<string, Itemmodel_s> itemmodel;
    void registerNode(const char* name, const char* texture_path[6]);
    void registerItem(const char* name, const char* texture_path);
};
#endif