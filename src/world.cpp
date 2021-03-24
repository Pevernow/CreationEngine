#include "world.h"
#include "noise.h"

#include <iostream>

Chunk::Chunk(int minx, int miny, int minz)
{
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                blocks[x][y][z].x = minx + x;
                blocks[x][y][z].y = miny + y;
                blocks[x][y][z].z = minz + z;
                float f =
                    simplex2((minx + x) * 0.05, (minz + z) * 0.05, 3, 0.5, 2);
                int h = (f + 1) / 2 * (16 - 1) + 1;
                for (int i = 0; i < h - 1; i++) {
                    blocks[x][i][z].type = "default_dirt";
                }
                blocks[x][h - 1][z].type = "default_grass";
            }
        }
    }
}

Block& World::get_node(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 || y > 15)
        return worldmap[0].blocks[0][0][0];
    for (int i = 0, l = worldmap.size(); i < l; i++) {
        if (worldmap[i].blocks[0][0][0].x <= x &&
            worldmap[i].blocks[0][0][0].y <= y &&
            worldmap[i].blocks[0][0][0].z <= z &&
            worldmap[i].blocks[15][15][15].x >= x &&
            worldmap[i].blocks[15][15][15].y >= y &&
            worldmap[i].blocks[15][15][15].z >= z) {
            // in chunk
            return worldmap[i].blocks[x % 16][y % 16][z % 16];
        }
    }
    // new chunk
    worldmap.push_back(Chunk(x - (x % 16), y - (y % 16), z - (z % 16)));
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

Chunk& World::get_chunk(int x, int y, int z)
{

    for (int i = 0, l = worldmap.size(); i < l; i++) {
        Chunk& chunk = worldmap[i];
        if (chunk.blocks[0][0][0].x <= x && chunk.blocks[0][0][0].y <= y &&
            chunk.blocks[0][0][0].z <= z && chunk.blocks[15][15][15].x >= x &&
            chunk.blocks[15][15][15].y >= y &&
            chunk.blocks[15][15][15].z >= z) {
            // in chunk
            return chunk;
        }
    }
    // not in chunk
    worldmap.push_back(Chunk(x - (x % 16), y - (y % 16), z - (z % 16)));
    return (Chunk&)worldmap[worldmap.size() - 1];
}

void World::generate_map()
{
    worldmap.push_back(Chunk(0, 0, 0));
}
/*
void World::generate_map()
{
    Chunk chunk(0, 0, 0);
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            float f = simplex2(x * 0.05, z * 0.05, 3, 0.5, 2);
            int h = (f + 1) / 2 * (16 - 1) + 1;
            for (int y = 0; y < h; y++) {
                chunk.blocks[x][y][z].type = "default_dirt";
            }
        }
    }
    worldmap.push_back(chunk);
}
*/