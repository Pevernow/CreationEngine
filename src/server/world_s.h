#ifndef _WORLD_S_H_
#define _WORLD_S_H_

#include "../world.h"

class World_s : public World
{
public:
    void set_node(int x, int y, int z, string type);
    void generate_map();
};
#endif