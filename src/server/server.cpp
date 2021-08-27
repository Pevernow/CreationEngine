#include "server.h"

#include <thread>

Server::Server() : net(&typemanager)
{
    world.generate_map();
    world.typemanager = &typemanager;
    typemanager.registerNode("air", nullptr);
    luaenv.init(&typemanager);
    luaenv.execmods();
}

void Server::shutdown()
{
    luaenv.destory();
    net.shutdown();
}

World* Server::getLocalWorldPtr()
{
    return &world;
}