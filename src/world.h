#ifndef WORLD_H
#define WORLD_H

#include "block.h"
#include <string>
#include <vector>
using namespace std;

class Chunk
{
public:
    Chunk(int minx, int miny, int minz);
    Block blocks[16][16][16];
    void update();
};
class World
{
public:
    Block& get_node(int x, int y, int z);
    Chunk& get_chunk(int x, int y, int z);
    TypeManager* typemanager;
    vector<Chunk> worldmap;
};
#endif