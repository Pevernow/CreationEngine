#ifndef WORLD_H
#define WORLD_H

#include "typemanager.h"
#include <vector>
using namespace std;

class Chunk
{
public:
    Chunk(int ix, int iy, int iz);
    Block* getBlockOrNullptr(int8_t local_x, int8_t local_y, int8_t local_z);
    void update();
    void updateBlock(int x, int y, int z);
    Block blocks[16][16][16];
    bool show;
    bool isInit;
};
class World
{
public:
    World();
    bool set_node(int x, int y, int z, const char* name);
    Block& get_node(int x, int y, int z);
    Chunk& get_chunk(int x, int y, int z);
    void updateLightForChunk(Chunk&);
    void updateTime(); // Call it every second
    string nodeIDtoItemID(int16_t nodeID);

    TypeManager* typemanager;
    vector<Chunk> worldmap;
    uint16_t time = 0;

private:
    int lastChunkIndex;
    void mapGenForChunk(Chunk& chunk);
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