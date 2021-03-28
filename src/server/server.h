#ifndef _SERVER_H_
#define _SERVER_H_

#include "../world.h"
#include "lua/mod.h"

class Server
{
public:
    void init();
    void shutdown();
    World* getLocalWorldPtr();
    TypeManager* getLocalTM();
    TypeManager typemanager;

private:
    World world;
    Luaenv luaenv;
};
#endif