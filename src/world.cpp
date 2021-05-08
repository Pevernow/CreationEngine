#include "world.h"
#include "noise.h"

#include <iostream>

void getChunkMinPosition(int& x, int& y, int& z)
{
    x = (x >= 0) ? x - (x % 16) : (x + 1) - ((x + 1) % 16) - 16;
    y = (y >= 0) ? y - (y % 16) : (y + 1) - ((y + 1) % 16) - 16;
    z = (z >= 0) ? z - (z % 16) : (z + 1) - ((z + 1) % 16) - 16;
    return;
}

Chunk::Chunk(int ix, int iy, int iz)
{
    show = false;
    getChunkMinPosition(ix, iy, iz);
    int minx = ix;
    int miny = iy;
    int minz = iz;
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            /*
            float f = simplex3(
                (minx + x) * 0.05, (miny + y) * 0.05, (minz + z) * 0.05, 4,
                0.6, 2);
            if (f > 0.5)
                blocks[x][y][z].id = 2;
            else if (f > 0)
                blocks[x][y][z].id = 1;
            else if (f <= 0)
                blocks[x][y][z].id = 0;
            */
            float f = simplex2((minx + x) * 0.03, (minz + z) * 0.03, 4, 0.5, 2);
            int h = (f + 1) / 2 * (16 - 1) - 5;
            for (int i = 0; i < h - 1; i++) {
                blocks[x][i][z].id = 1;
            }
            blocks[x][h - 1][z].id = 2;
            for (int y = 0; y < 16; y++) {
                blocks[x][y][z].x = minx + x;
                blocks[x][y][z].y = miny + y;
                blocks[x][y][z].z = minz + z;
                blocks[x][y][z].show = false;
            }
        }
    }
    update();
}

Block& World::get_node(int x, int y, int z)
{
    if (y > 15 || y < 0)
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
    worldmap.push_back(Chunk(x, y, z));
    return worldmap[worldmap.size() - 1].blocks[x % 16][y % 16][z % 16];
}

void World::set_node(int x, int y, int z, const char* name)
{
    if (x < 0 || y < 0 || z < 0 || y > 15)
        return;
    for (int i = 0, l = worldmap.size(); i < l; i++) {
        if (worldmap[i].blocks[0][0][0].x <= x &&
            worldmap[i].blocks[0][0][0].y <= y &&
            worldmap[i].blocks[0][0][0].z <= z &&
            worldmap[i].blocks[15][15][15].x >= x &&
            worldmap[i].blocks[15][15][15].y >= y &&
            worldmap[i].blocks[15][15][15].z >= z) {
            // in chunk
            worldmap[i].blocks[x % 16][y % 16][z % 16].id =
                typemanager->nameToID(name);
            worldmap[i].update();
            return;
        }
    }
}

Chunk& World::get_chunk(int x, int y, int z)
{
    getChunkMinPosition(x, y, z);
    for (int i = 0, l = worldmap.size(); i < l; i++) {
        Chunk& chunk = worldmap[i];
        if (chunk.blocks[0][0][0].x == x && chunk.blocks[0][0][0].y == y &&
            chunk.blocks[0][0][0].z == z) {
            // in chunk
            return chunk;
        }
    }
    // not in chunk
    worldmap.push_back(Chunk(x, y, z));
    return (Chunk&)worldmap[worldmap.size() - 1];
}

void Chunk::update()
{
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                if (blocks[x][y][z].id == 0) {
                    blocks[x][y][z].show = false;
                    continue;
                }
                if (x - 1 >= 0 && blocks[x - 1][y][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (x + 1 < 16 && blocks[x + 1][y][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (y - 1 >= 0 && blocks[x][y - 1][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (y + 1 < 16 && blocks[x][y + 1][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (z - 1 >= 0 && blocks[x][y][z - 1].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (z + 1 < 16 && blocks[x][y][z + 1].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
            }
        }
    }
}

// bad code
/*
void Chunk::updateBlock(int x, int y, int z)
{
    for (int ix = -1; ix <= 1; ix++) {
        for (int iy = -1; iy <= 1; iy++) {
            for (int iz = -1; iz <= 1; iz++) {
                if (x + ix < 0 || x + ix > 15 || y + iy < 0 || y + iy > 15 ||
                    z + iz < 0 || z + iz > 15) {
                    continue;
                }
                if (blocks[x][y][z].id == 0) {
                    blocks[x][y][z].show = false;
                    continue;
                }
                if (blocks[ix + x - 1][y][z].id == 0) {
                    blocks[ix + x][iy + y][z].show = true;
                    continue;
                }
                if (blocks[x + 1][y][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (blocks[x][y - 1][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (blocks[x][y + 1][z].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (blocks[x][y][z - 1].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
                if (blocks[x][y][z + 1].id == 0) {
                    blocks[x][y][z].show = true;
                    continue;
                }
            }
        }
    }
}
*/
World::World()
{
    seed(888);
}
/*
Mapgen::Mapgen(unsigned int worldSeed)
{
    seed(worldSeed);
}
*/