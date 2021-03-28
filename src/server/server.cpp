#include "server.h"

void Server::init()
{
    world.generate_map();
    world.typemanager = &typemanager;
    typemanager.registerNode("air", nullptr);
    luaenv.init();
    luaenv.execmods();
}

void Server::shutdown()
{
    luaenv.destory();
}

World* Server::getLocalWorldPtr()
{
    return &world;
}

TypeManager* Server::getLocalTM()
{
    return &typemanager;
}