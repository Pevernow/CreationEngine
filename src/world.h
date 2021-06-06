#ifndef WORLD_H
#define WORLD_H

#include "block.h"
#include <deque>
using namespace std;

class Chunk
{
public:
    Chunk(int ix, int iy, int iz);
    Block blocks[16][16][16];
    void update();
    void updateBlock(int x, int y, int z);
    bool show;
    bool isInit;
};
class World
{
public:
    World();
    void set_node(int x, int y, int z, const char* name);
    Block& get_node(int x, int y, int z);
    Chunk& get_chunk(int x, int y, int z);

    TypeManager* typemanager;
    deque<Chunk> worldmap;

private:
    void mapGenForChunk(Chunk&);
    bool isAroundTree(Chunk&, int x, int y, int z);
};
void getChunkMinPosition(int& x, int& y, int& z);
/*
class Mapgen
{
public:
    Mapgen(unsigned int worldSeed);
    void generateChunk(Chunk& chunk);

private:
    int getBiomefromXY(short x, short y);
    void getHeightfromXY(short x, short y);
};
*/
#endif