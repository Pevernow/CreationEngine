#include "server.h"

#include <thread>

void Server::init()
{
    world.generate_map();
    world.typemanager = &typemanager;
    typemanager.registerNode("air", nullptr);
    luaenv.init(&typemanager);
    luaenv.execmods();
    net.init(&typemanager);
    net.eventloop();
}

void Server::shutdown()
{
    luaenv.destory();
}

World* Server::getLocalWorldPtr()
{
    return &world;
}