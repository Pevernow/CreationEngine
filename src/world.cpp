#include "world.h"

extern bool quit;
extern World world;
extern map<string, BlockType> blocktypes;


Chunk::Chunk(glm::vec3 min){
    for(int x=0;x<16;x++){
        for(int y=0;y<16;y++){
            for(int z=0;z<16;z++){
                blocks[x][y][z].pos = min+glm::vec3(x,y,z);
            }
        }
    }
}

Block& World::get_node(int x,int y,int z){
    for(int i=0,l=worldmap.size();i<l;i++){
        if (worldmap[i].blocks[0][0][0].pos.x<=x&&worldmap[i].blocks[0][0][0].pos.y<=y&&worldmap[i].blocks[0][0][0].pos.z<=z&&worldmap[i].blocks[15][15][15].pos.x>=x&&worldmap[i].blocks[15][15][15].pos.y>=y&&worldmap[i].blocks[15][15][15].pos.z>=z){
            //in chunk
            return worldmap[i].blocks[int(x)%16][int(y)%16][int(z)%16];
        }
    }
    worldmap.push_back(Chunk(glm::vec3(int(x)%16,int(y)%16,int(z)%16)));
    return worldmap[worldmap.size()-1].blocks[int(x)%16][int(y)%16][int(z)%16];
}

Block& World::get_node(glm::vec3 pos){
    return this->get_node(pos.x,pos.y,pos.z);
}

void World::set_node(int x,int y,int z,string type){
    for(int i=0,l=worldmap.size();i<l;i++){
        if(worldmap[i].blocks[0][0][0].pos.x<=x&&worldmap[i].blocks[0][0][0].pos.y<=y&&worldmap[i].blocks[0][0][0].pos.z<=z&&
        worldmap[i].blocks[15][15][15].pos.x>=x&&worldmap[i].blocks[15][15][15].pos.y>=y&&worldmap[i].blocks[15][15][15].pos.z>=z){
            //in chunk
            worldmap[i].blocks[int(x)%16][int(y)%16][int(z)%16].type = type;
            return;
        }
    }
    worldmap.push_back(Chunk(glm::vec3(int(x)%16,int(y)%16,int(z)%16)));
    worldmap[worldmap.size()-1].blocks[int(x)%16][int(y)%16][int(z)%16].type = type;
    blocktypes[type].add(glm::vec3(x,y,z));
    return;
}

void genMap(){
    int test=0;
    for(int x=0;x<10;x++){
        for(int z=0;z<10;z++){
            test++;
            world.set_node(x,0,z,"default:dirt");
        }
    }
}