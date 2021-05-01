#ifndef WORLD_H
#define WORLD_H

#include "block.h"
#include <vector>
using namespace std;

class Chunk
{
public:
    Chunk(int ix, int iy, int iz);
    Block blocks[16][16][16];
    void update();
    void updateBlock(int x, int y, int z);
    bool show;
};
class World
{
public:
    World();
    void set_node(int x, int y, int z, const char* name);
    Block& get_node(int x, int y, int z);
    Chunk& get_chunk(int x, int y, int z);
    TypeManager* typemanager;
    vector<Chunk> worldmap;
};
#endif