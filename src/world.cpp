#include "world.h"
#include "noise.h"

#include <cstdlib>
#include <iostream>

void getChunkMinPosition(int& x, int& y, int& z)
{
    x = (x >= 0) ? x - (x % 16) : (x + 1) - ((x + 1) % 16) - 16;
    y = (y >= 0) ? y - (y % 16) : (y + 1) - ((y + 1) % 16) - 16;
    z = (z >= 0) ? z - (z % 16) : (z + 1) - ((z + 1) % 16) - 16;
    return;
}

int getPositionInChunk(int a)
{
    if (a >= 0) {
        return a % 16;
    } else {
        return 15 + ((a + 1) % 16);
    }
}

Chunk::Chunk(int ix, int iy, int iz)
{
    isInit = false;
    show = false;
    getChunkMinPosition(ix, iy, iz);
    blocks[0][0][0].x = ix;
    blocks[0][0][0].y = iy;
    blocks[0][0][0].z = iz;
}

void World::mapGenForChunk(Chunk& chunk)
{
    int treeCount = 0;

    int minx = chunk.blocks[0][0][0].x;
    int miny = chunk.blocks[0][0][0].y;
    int minz = chunk.blocks[0][0][0].z;
    for (int x = 0; x < 16; x++) {
        for (int z = 0; z < 16; z++) {
            // Generate height map
            float f = simplex2((minx + x) * 0.01, (minz + z) * 0.01, 2, 0.2, 2);
            int h = (f + 1.0) / 2.0 * (32 - 1);

            // Generate heat
            f = simplex2((minx + x) * 0.01, (minz + z) * 0.01, 1, 0.5, 2);
            int heat = f * 50 - h;

            f = simplex2((minx + x) * 0.001, (minz + z) * 0.001, 1, 0.5, 2);
            float humidity = (f + 1) / 2;

            // Decide biome by heat and humidity
            int groundID = 0;
            int leaveID = 4;
            int treeID = 3;

            // Grass
            if (heat < 50 && humidity > 0) {
                groundID = 2;
            }

            for (int y = 0; y < 16; y++) {
                chunk.blocks[x][y][z].x = minx + x;
                chunk.blocks[x][y][z].y = miny + y;
                chunk.blocks[x][y][z].z = minz + z;
                chunk.blocks[x][y][z].show = false;
            }
            if (h < miny) {
                continue;
            } else if (h >= miny + 15) {
                for (int i = 0; i <= 15; i++) {
                    chunk.blocks[x][i][z].id = 1;
                }
            } else {
                int has_water = false;
                if (h < 20) {
                    for (int i = h % 16; i <= 4; i++) {
                        chunk.blocks[x][i][z].id = 5;
                    }
                    has_water = true;
                }
                h = h % 16;
                for (int i = 0; i <= h - 1; i++) {
                    chunk.blocks[x][i][z].id = 1;
                }
                chunk.blocks[x][h][z].id = groundID;

                if (has_water)
                    continue;

                // Generate tree
                if (h + 5 < 32 && x - 1 >= 0 && x + 1 < 16 && z - 1 >= 0 &&
                    z + 1 < 16 && treeCount < 3 && rand() % 50 == 37) {
                    // leaves
                    treeCount++;
                    for (int i = 3; i <= 5; i++) {
                        for (int j = -1; j <= 1; j++) {
                            for (int k = -1; k <= 1; k++) {
                                chunk.blocks[x + j][h + i][z + k].id = leaveID;
                            }
                        }
                    }
                    // truck
                    for (int i = 1; i <= 5; i++) {
                        chunk.blocks[x][h + i][z].id = treeID;
                    }
                }
            }
        }
    }

    chunk.update();
    chunk.isInit = true;
}

bool World::isAroundTree(Chunk& chunk, int x, int y, int z)
{
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                // checked edges before
                // do not check again
                if (y + j < 16 && y - j > 0) {
                    int id = chunk.blocks[x + i][y + j][x + k].id;
                    if (id == 4 || id == 3) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Block& World::get_node(int x, int y, int z)
{

    if (y < 0)
        return worldmap[0].blocks[0][0][0];

    // Test lastIndex first
    if (lastChunkIndex != -1) {
        Chunk& lastChunk = worldmap[lastChunkIndex];
        if (lastChunk.blocks[0][0][0].x <= x &&
            lastChunk.blocks[0][0][0].y <= y &&
            lastChunk.blocks[0][0][0].z <= z &&
            lastChunk.blocks[15][15][15].x >= x &&
            lastChunk.blocks[15][15][15].y >= y &&
            lastChunk.blocks[15][15][15].z >= z) {
            // in chunk
            return lastChunk.blocks[getPositionInChunk(x)][getPositionInChunk(
                y)][getPositionInChunk(z)];
        }
    }

    for (int i = 0, l = worldmap.size(); i < l; i++) {
        Chunk& testChunk = worldmap[i];
        if (testChunk.blocks[0][0][0].x <= x &&
            testChunk.blocks[0][0][0].y <= y &&
            testChunk.blocks[0][0][0].z <= z &&
            testChunk.blocks[15][15][15].x >= x &&
            testChunk.blocks[15][15][15].y >= y &&
            testChunk.blocks[15][15][15].z >= z) {
            // in chunk
            lastChunkIndex = i;
            return testChunk.blocks[getPositionInChunk(x)][getPositionInChunk(
                y)][getPositionInChunk(z)];
        }
    }
    // new chunk
    worldmap.emplace_back(Chunk(x, y, z));
    Chunk& new_chunk = worldmap.back();
    mapGenForChunk(new_chunk);
    lastChunkIndex = worldmap.size() - 1;
    return new_chunk.blocks[x % 16][y % 16][z % 16];
}

void World::set_node(int x, int y, int z, const char* name)
{
    if (x < 0 || y < 0 || z < 0)
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
    worldmap.emplace_back(Chunk(x, y, z));
    Chunk& new_chunk = worldmap.back();
    mapGenForChunk(new_chunk);
    return new_chunk;
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
                if (x + ix < 0 || x + ix > 15 || y + iy < 0 || y + iy > 15
|| z + iz < 0 || z + iz > 15) { continue;
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
    lastChunkIndex = -1;
    seed(888);
    srand(888);
}
/*
Mapgen::Mapgen(unsigned int worldSeed)
{
    seed(worldSeed);
}
*/