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
};
class World
{
public:
    Block& get_node(int x, int y, int z);
    void set_node(int x, int y, int z, string type);
    Chunk& get_chunk(int x, int y, int z);
    void generate_map();
    vector<Chunk> worldmap;
};
#endif