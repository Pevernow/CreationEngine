#include "world.h"

Chunk::Chunk(int minx, int miny, int minz)
{
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                blocks[x][y][z].x = minx + x;
                blocks[x][y][z].y = miny + y;
                blocks[x][y][z].z = minz + z;
            }
        }
    }
}

Block& World::get_node(int x, int y, int z)
{
    for (int i = 0, l = worldmap.size(); i < l; i++) {
        if (worldmap[i].blocks[0][0][0].x <= x &&
            worldmap[i].blocks[0][0][0].y <= y &&
            worldmap[i].blocks[0][0][0].z <= z &&
            worldmap[i].blocks[15][15][15].x >= x &&
            worldmap[i].blocks[15][15][15].y >= y &&
            worldmap[i].blocks[15][15][15].z >= z) {
            // in chunk
            return worldmap[i].blocks[int(x) % 16][int(y) % 16][int(z) % 16];
        }
    }
    // new chunk
    worldmap.push_back(Chunk((x % 16) * 16, (y % 16) * 16, (z % 16) * 16));
    return worldmap[worldmap.size() - 1].blocks[x % 16][y % 16][z % 16];
}

void World::set_node(int x, int y, int z, string type)
{
    if (is_type_registed(type) == false) {
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
            worldmap[i].blocks[x % 16][y % 16][z % 16].type = type;
            return;
        }
    }
    worldmap.push_back(Chunk((x % 16) * 16, (y % 16) * 16, (z % 16) * 16));
    worldmap[worldmap.size() - 1].blocks[x % 16][y % 16][z % 16].type = type;
    return;
}

void World::mapgen()
{
    worldmap.push_back(Chunk(0, 0, 0));
    worldmap.push_back(Chunk(0, 0, 16));
}