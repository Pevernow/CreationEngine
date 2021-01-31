#ifndef _MAPGEN_H_
#define _MAPGEN_H_

float simplex2(
    float x, float y, int octaves, float persistence, float lacunarity);

float simplex3(
    float x, float y, float z, int octaves, float persistence,
    float lacunarity);
#endif // _MAPGEN_H_