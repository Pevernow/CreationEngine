#ifndef _NETWORK_C_H_
#define _NETWORK_C_H_
#include "typemanager_c.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

class Network_c
{
public:
    void init(TypeManager_c* tmPtr, int port = 6180);
    void eventloop();
    void startUp();

private:
    struct event_base* base;
    struct sockaddr_in serv;
    struct bufferevent* bev;
};
#endif