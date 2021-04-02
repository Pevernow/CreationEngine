#include "world_s.h"

void World_s::set_node(int x, int y, int z, string type)
{
    int id = typemanager->nameToID(type);
    if (id == -1) {
        return;
    }
    for (int i = 0, l = worldmap.size(); i < l; i++) {
        if (worldmap[i].blocks[0][0][0].x <= x &&
            worldmap[i].blocks[0][0][0].y <= y &&
            worldmap[i].blocks[0][0][0].z <= z &&
            worldmap[i].blocks[15][15][15].x >= x &&
            worldmap[i].blocks[15][15][15].y >= y &&
            worldmap[i].blocks[15][15][15].z >= z) {
            // in chunk
            worldmap[i].blocks[x % 16][y % 16][z % 16].id = id;
            return;
        }
    }
    worldmap.push_back(Chunk((x % 16) * 16, (y % 16) * 16, (z % 16) * 16));
    worldmap[worldmap.size() - 1].blocks[x % 16][y % 16][z % 16].id = id;
    return;
}

void World_s::generate_map()
{
    worldmap.push_back(Chunk(0, 0, 0));
}