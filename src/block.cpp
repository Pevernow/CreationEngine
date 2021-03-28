#include "block.h"
#include "file-ops.h"
#include "utils.h"
#include "world.h"
#include <bx/math.h>
#include <iostream>

using namespace std;

Blockmodel::Blockmodel(
    string blockName, uint16_t blockID, const char* texture_path)
{
    name = blockName;
    id = blockID;
    if (blockID != 0)
        textureData = loadTexture(texture_path);
    // textureData = bgfx::TextureHandle();
}

void TypeManager::registerNode(const char* name, const char* texture_path)
{
    int id = blockmodel.size();
    nameIndex[name] = id;
    blockmodel.push_back(Blockmodel(name, id, texture_path));
    cout << name << " registed" << endl;
}

int TypeManager::nameToID(string name)
{
    map<string, int>::iterator iter = nameIndex.find(name);
    if (iter != nameIndex.end()) {
        return iter->second;
    }
    return -1;
}