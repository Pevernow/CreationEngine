#ifndef _NETWORK_S_H_
#define _NETWORK_S_H_
#include "block_s.h"
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

class Network_s
{
public:
    void init(TypeManager_s* tmPtr, int port = 6180);
    void eventloop();

private:
    struct event_base* base;
    struct sockaddr_in serv;
    struct evconnlistener* listener;
};
#endif