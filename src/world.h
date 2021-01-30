#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Node.h"
using namespace std;

class Chunk{
    public:
    Chunk(glm::vec3 min);
    Block blocks[16][16][16];
};
class World{
    public:
    Block& get_node(int x,int y,int z);
    Block& get_node(glm::vec3 pos);
    void set_node(int x,int y,int z,string type);
    private:
    vector<Chunk> worldmap;
};

void genMap();
#endif