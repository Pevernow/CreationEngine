#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <bgfx/bgfx.h>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Block
{
public:
    int16_t x;
    int16_t y;
    int16_t z;
    uint16_t id = 0;
    bool show;
    uint8_t sun_light = 15;
};

class ItemStack
{
public:
    ItemStack();
    ItemStack(string id, int num);
    string id;
    uint16_t num;
};

class Inventory
{
public:
    vector<ItemStack> items;
};

class TypeManager
{
public:
    int nameToID(string name); // use id to index map of blockmodels.
    string idToName(int id);
    map<string, int> nameIndex;
};

#endif