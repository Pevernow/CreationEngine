#ifndef _SERVER_H_
#define _SERVER_H_

#include "lua/mod.h"
#include "world_s.h"

class Server
{
public:
    void init();
    void shutdown();
    World* getLocalWorldPtr();
    TypeManager* getLocalTM();
    TypeManager typemanager;

private:
    World_s world;
    Luaenv luaenv;
};
#endif