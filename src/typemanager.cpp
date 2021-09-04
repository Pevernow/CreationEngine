#include "typemanager.h"
#include "world.h"
#include <iostream>

using namespace std;

int TypeManager::nameToID(string name)
{
    map<string, int>::iterator iter = nameIndex.find(name);
    if (iter != nameIndex.end()) {
        return iter->second;
    }
    return -1;
}

string TypeManager::idToName(int id)
{
    map<string, int>::iterator iter = nameIndex.begin();
    while (iter != nameIndex.end()) {
        if (iter->second == id) {
            return iter->first;
        }
        iter++;
    }
    return "undefined";
}

ItemStack::ItemStack()
{
    id = "!empty";
    num = -1;
}

ItemStack::ItemStack(string id, int num)
{
    this->id = id;
    this->num = num;
}

uint8_t ItemStack::get(int n)
{
    if (n >= num) {
        id = "!empty";
        int tmp = num;
        num = 0;
        return tmp;
    } else {
        num -= n;
        return n;
    }
}

uint8_t ItemStack::put(int n)
{
    if (n + num >= 255) {
        int tmp = num;
        num = 255;
        return 255 - tmp;
    } else {
        num += n;
        return n;
    }
}

bool Inventory::putItem(ItemStack item)
{
    for (int i = 0, l = items.size(); i < l; i++) {
        ItemStack& it = items[i];
        if (it.id == item.id) {
            int ret = it.put(item.num);
            item.get(ret);
            if (item.num == 0) {
                return true;
            }
        }
    }
    return false;
}