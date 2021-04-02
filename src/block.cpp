#include "block.h"
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